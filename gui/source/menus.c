#include <stdarg.h>
#include <stdlib.h>

#include <3ds.h>

#include "ac.h"
#include "actu.h"
#include "C2D_helper.h"
#include "common.h"
#include "config.h"
#include "hardware.h"
#include "kernel.h"
#include "menu_control.h"
#include "misc.h"
#include "screenshot.h"
#include "storage.h"
#include "system.h"
#include "textures.h"
#include "utils.h"
#include "wifi.h"

#define DISTANCE_Y  20
#define MENU_Y_DIST 18
#define MAX_ITEMS   9

static bool display_info = false;
static int item_height = 0;
static char kernel_version[100], system_version[100], firm_version[100], initial_version[0xB], nand_lfcs[0xB];
static u32 sd_titles = 0, nand_titles = 0, tickets = 0;

static void Menu_DrawItem(int x, int y, char *item_title, const char* text, ...)
{
	float title_width = 0.0f;
	Draw_GetTextSize(0.5f, &title_width, NULL, item_title);
	Draw_Text(x, y, 0.5f, MENU_INFO_TITLE_COLOUR, item_title);
	
	char buffer[256];
	va_list args;
	va_start(args, text);
	vsnprintf(buffer, 256, text, args);
	Draw_Text(x + title_width + 5, y, 0.5f, MENU_INFO_DESC_COLOUR, buffer);
	va_end(args);
}

static void Menu_Kernel(void)
{
	Menu_DrawItem(15, 102, "Kernel version:", kernel_version);
	Menu_DrawItem(15, 120, "FIRM version:", firm_version);
	Menu_DrawItem(15, 136, "System version:", system_version);
	Menu_DrawItem(15, 156, "Initial system version:", initial_version);
	Menu_DrawItem(15, 174, "SDMC CID:", display_info? Kernel_GetSDMCCID() : NULL);
	Menu_DrawItem(15, 192, "NAND CID:", display_info? Kernel_GetNANDCID() : NULL);
	Menu_DrawItem(15, 210, "Device ID:", "%llu", display_info? Kernel_GetDeviceId() : 0);
}

static void Menu_System(void)
{
	Menu_DrawItem(15, 102, "Model:", "%s (%s - %s)", System_GetModel(), System_GetRunningHW(), System_GetRegion());
	Menu_DrawItem(15, 120, "Language:", System_GetLang());
	Menu_DrawItem(15, 138, "ECS Device ID:", "%llu", display_info? System_GetSoapId() : 0);
	Menu_DrawItem(15, 156, "Original local friend code seed:", "%010llX", display_info? System_GetLocalFriendCodeSeed() : 0);
	Menu_DrawItem(15, 174, "NAND local friend code seed:", "%s", display_info? nand_lfcs : NULL);
	Menu_DrawItem(15, 192, "MAC Address:", display_info? System_GetMacAddress() : NULL);
	Menu_DrawItem(15, 210, "Serial number:", display_info? System_GetSerialNumber() : NULL);
}

static void Menu_Battery(void)
{
	Result ret = 0;
	u8 battery_percent = 0, battery_status = 0, battery_volt = 0, fw_ver_high = 0, fw_ver_low = 0;
	bool is_connected = false;

	ret = MCUHWC_GetBatteryLevel(&battery_percent);
	Menu_DrawItem(15, 102, "Battery percentage:", "%3d%%", R_FAILED(ret)? 0 : (battery_percent));
	
	ret = PTMU_GetBatteryChargeState(&battery_status);
	Menu_DrawItem(15, 120, "Battery status:", R_FAILED(ret)? NULL : (battery_status? "charging" : "not charging"));
	
	if (R_FAILED(ret = MCUHWC_GetBatteryVoltage(&battery_volt)))
		Menu_DrawItem(15, 136, "Battery voltage:", "%d (%.1f V)", 0, 0);
	else
		Menu_DrawItem(15, 136, "Battery voltage:", "%d (%.1f V)", battery_volt, 5.0 * ((double)battery_volt / 256.0));

	ret = PTMU_GetAdapterState(&is_connected);
	Menu_DrawItem(15, 156, "Adapter state:", R_FAILED(ret)? NULL : (is_connected? "connected" : "disconnected"));

	if ((R_FAILED(ret = MCUHWC_GetFwVerHigh(&fw_ver_high))) && (R_FAILED(ret = MCUHWC_GetFwVerLow(&fw_ver_low))))
		Menu_DrawItem(15, 174, "MCU firmware:", "0.0");
	else
		Menu_DrawItem(15, 174, "MCU firmware:", "%u.%u", (fw_ver_high - 0x10), fw_ver_low);

	Menu_DrawItem(15, 192, "Power-saving mode:", Config_IsPowerSaveEnabled()? "enabled" : "disabled");
}

static void Menu_NNID(void)
{
	Result ret = 0;
	AccountDataBlock accountDataBlock;
	Result accountDataBlockRet = ACTU_GetAccountDataBlock((u8*)&accountDataBlock, 0xA0, 0x11);

	u32 principalID = 0;
	char country[0x3], name[0x16], nnid[0x11], timeZone[0x41];
	
	ret = ACTU_GetAccountDataBlock(nnid, 0x11, 0x8);
	Menu_DrawItem(15, 102, "NNID:", R_FAILED(ret)? NULL : (display_info? nnid : NULL));

	ret = ACTU_GetAccountDataBlock(&principalID, 0x4, 0xC);
	Menu_DrawItem(15, 120, "Principal ID:", "%u", R_FAILED(ret)? 0 : (display_info? principalID : 0));

	Menu_DrawItem(15, 136, "Persistent ID:", "%u", R_FAILED(accountDataBlockRet)? 0 : (display_info? accountDataBlock.persistentID : 0));
	Menu_DrawItem(15, 156, "Transferable ID Base:", "%llu", R_FAILED(accountDataBlockRet)? 0 : (display_info? accountDataBlock.transferableID : 0));
	
	ret = ACTU_GetAccountDataBlock(country, 0x3, 0xB);
	Menu_DrawItem(15, 174, "Country:", R_FAILED(ret)? NULL : (display_info? country : NULL));
	
	ret = ACTU_GetAccountDataBlock(timeZone, 0x41, 0x1E);
	Menu_DrawItem(15, 192, "Time Zone:", R_FAILED(ret)? NULL : (display_info? timeZone : NULL));
}

static void Menu_Config(void)
{
	char username[0x14];
	wcstombs(username, Config_GetUsername(), sizeof(username));

	Menu_DrawItem(15, 102, "Username: ", username);
	Menu_DrawItem(15, 120, "Birthday:", display_info? Config_GetBirthday() : NULL);
	Menu_DrawItem(15, 136, "EULA version:", Config_GetEulaVersion());
	Menu_DrawItem(15, 156, "Parental control pin:", display_info? Config_GetParentalPin() : NULL);
	Menu_DrawItem(15, 174, "Parental control e-mail:", display_info? Config_GetParentalEmail() : NULL);
	Menu_DrawItem(15, 192, "Parental control answer:", display_info? Config_GetParentalSecretAnswer() : NULL);
}

static void Menu_Hardware(void)
{
	Result ret = 0;

	Menu_DrawItem(15, 102, "Screen type:", System_GetScreenType());
	Menu_DrawItem(15, 120, "Headphone status:", Hardware_GetAudioJackStatus());
	Menu_DrawItem(15, 136, "Card slot status:", Hardware_GetCardSlotStatus());
	Menu_DrawItem(15, 156, "SDMC status:", Hardware_DetectSD());

	Menu_DrawItem(15, 174, "Sound output:", Config_GetSoundOutputMode());

	if (Utils_IsN3DS())
	{
		Menu_DrawItem(15, 192, "Brightness level:", "%s (auto-brightness mode %s)", Hardware_GetBrightness(GSPLCD_SCREEN_TOP), 
			Config_IsAutoBrightnessEnabled()? "enabled" : "disabled");
	}
	else
		Menu_DrawItem(15, 192, "Brightness level:", Hardware_GetBrightness(GSPLCD_SCREEN_TOP));

}

static void Menu_Misc(void)
{
	Result ret = 0;
	Menu_DrawItem(15, 102, "Installed titles:", "SD: %lu (NAND: %lu)", sd_titles, nand_titles);
	Menu_DrawItem(15, 120, "Installed tickets:", "%lu", tickets);

	u64 homemenuID = 0;
	ret = APT_GetAppletInfo(APPID_HOMEMENU, &homemenuID, NULL, NULL, NULL, NULL);
	Menu_DrawItem(15, 136, "Homemenu ID:", "%016llX", homemenuID);

	double wifi_signal_percent = (osGetWifiStrength() * 33.3333333333);
	Menu_DrawItem(15, 156, "WiFi signal strength:", "%d (%.0lf%%)", osGetWifiStrength(), wifi_signal_percent);
	
	u32 ip = gethostid();
	if (display_info)
		Menu_DrawItem(15, 174, "IP:", "%lu.%lu.%lu.%lu", ip & 0xFF, (ip>>8)&0xFF, (ip>>16)&0xFF, (ip>>24)&0xFF);
	else
		Menu_DrawItem(15, 174, "IP:", "%lu.%lu.%lu.%lu", 0, 0, 0, 0);

}

static void Menu_WiFi(void)
{
	char ssid[0x20], passphrase[0x40];
	wifiSlotStructure slotData;

	Draw_Rect(0, 19, 400, 221, BACKGROUND_COLOUR);
	
	if (R_SUCCEEDED(ACI_LoadWiFiSlot(0)))
	{
		Draw_Rect(15, 27, 370, 70, MENU_INFO_TITLE_COLOUR);
		Draw_Rect(16, 28, 368, 68, MENU_BAR_COLOUR);
		
		Draw_Text(20, 30, 0.45f, MENU_INFO_DESC_COLOUR, "WiFi Slot 1:");

		if (R_SUCCEEDED(ACI_GetSSID(ssid)))
			Menu_DrawItem(20, 46, "SSID:", ssid);
		
		if (R_SUCCEEDED(ACI_GetPassphrase(passphrase)))
			Menu_DrawItem(20, 62, "Pass:", "%s (%s)", display_info? passphrase : NULL, WiFi_GetSecurityMode());

		if ((R_SUCCEEDED(CFG_GetConfigInfoBlk8(CFG_WIFI_SLOT_SIZE, CFG_WIFI_BLKID + 0, (u8*)&slotData))) && (slotData.set))
		{
			if (display_info)
				Menu_DrawItem(20, 78, "Mac address:", "%02X:%02X:%02X:%02X:%02X:%02X", slotData.mac_addr[0], slotData.mac_addr[1], slotData.mac_addr[2], 
					slotData.mac_addr[3], slotData.mac_addr[4], slotData.mac_addr[5]);
			else
				Menu_DrawItem(20, 78, "Mac address:", NULL);
		}
	}
	
	if (R_SUCCEEDED(ACI_LoadWiFiSlot(1)))
	{
		Draw_Rect(15, 95, 370, 70, MENU_INFO_TITLE_COLOUR);
		Draw_Rect(16, 96, 368, 68, MENU_BAR_COLOUR);
		
		Draw_Text(20, 98, 0.45f, MENU_INFO_DESC_COLOUR, "WiFi Slot 2:");

		if (R_SUCCEEDED(ACI_GetSSID(ssid)))
			Menu_DrawItem(20, 114, "SSID:", ssid);
		
		if (R_SUCCEEDED(ACI_GetPassphrase(passphrase)))
			Menu_DrawItem(20, 130, "Pass:", "%s (%s)", display_info? passphrase : NULL, WiFi_GetSecurityMode());

		if ((R_SUCCEEDED(CFG_GetConfigInfoBlk8(CFG_WIFI_SLOT_SIZE, CFG_WIFI_BLKID + 1, (u8*)&slotData))) && (slotData.set))
		{
			if (display_info)
				Menu_DrawItem(20, 146, "Mac address:", "%02X:%02X:%02X:%02X:%02X:%02X", slotData.mac_addr[0], slotData.mac_addr[1], slotData.mac_addr[2], 
					slotData.mac_addr[3], slotData.mac_addr[4], slotData.mac_addr[5]);
			else
				Menu_DrawItem(20, 146, "Mac address:", NULL);
		}
	}
	
	if (R_SUCCEEDED(ACI_LoadWiFiSlot(2)))
	{
		Draw_Rect(15, 163, 370, 70, MENU_INFO_TITLE_COLOUR);
		Draw_Rect(16, 164, 368, 68, MENU_BAR_COLOUR);
		
		Draw_Text(20, 166, 0.45f, MENU_INFO_DESC_COLOUR, "WiFi Slot 3:");

		if (R_SUCCEEDED(ACI_GetSSID(ssid)))
			Menu_DrawItem(20, 182, "SSID:", ssid);
		
		if (R_SUCCEEDED(ACI_GetPassphrase(passphrase)))
			Menu_DrawItem(20, 198, "Pass:", "%s (%s)", display_info? passphrase : NULL, WiFi_GetSecurityMode());

		if ((R_SUCCEEDED(CFG_GetConfigInfoBlk8(CFG_WIFI_SLOT_SIZE, CFG_WIFI_BLKID + 2, (u8*)&slotData))) && (slotData.set))
		{
			if (display_info)
				Menu_DrawItem(20, 214, "Mac address:", "%02X:%02X:%02X:%02X:%02X:%02X", slotData.mac_addr[0], slotData.mac_addr[1], slotData.mac_addr[2], 
					slotData.mac_addr[3], slotData.mac_addr[4], slotData.mac_addr[5]);
			else
				Menu_DrawItem(20, 214, "Mac address:", NULL);
		}
	}
}

static void Menu_Storage(void)
{
	u64 sdUsed = 0, sdTotal = 0, ctrUsed = 0, ctrTotal = 0, twlUsed = 0, twlTotal = 0, twlpUsed = 0, twlpTotal = 0; 

	char sdFreeSize[16], sdUsedSize[16], sdTotalSize[16];
	char ctrFreeSize[16], ctrUsedSize[16], ctrTotalSize[16];
	char twlFreeSize[16], twlUsedSize[16], twlTotalSize[16];
	char twlpFreeSize[16], twlpUsedSize[16], twlpTotalSize[16];

	Utils_GetSizeString(sdFreeSize, Storage_GetFreeStorage(SYSTEM_MEDIATYPE_SD));
	Utils_GetSizeString(sdUsedSize, Storage_GetUsedStorage(SYSTEM_MEDIATYPE_SD));
	Utils_GetSizeString(sdTotalSize, Storage_GetTotalStorage(SYSTEM_MEDIATYPE_SD));
	Utils_GetSizeString(ctrFreeSize, Storage_GetFreeStorage(SYSTEM_MEDIATYPE_CTR_NAND));
	Utils_GetSizeString(ctrUsedSize, Storage_GetUsedStorage(SYSTEM_MEDIATYPE_CTR_NAND));
	Utils_GetSizeString(ctrTotalSize, Storage_GetTotalStorage(SYSTEM_MEDIATYPE_CTR_NAND));
	Utils_GetSizeString(twlFreeSize, Storage_GetFreeStorage(SYSTEM_MEDIATYPE_TWL_NAND));
	Utils_GetSizeString(twlUsedSize, Storage_GetUsedStorage(SYSTEM_MEDIATYPE_TWL_NAND));
	Utils_GetSizeString(twlTotalSize, Storage_GetTotalStorage(SYSTEM_MEDIATYPE_TWL_NAND));
	Utils_GetSizeString(twlpFreeSize, Storage_GetFreeStorage(SYSTEM_MEDIATYPE_TWL_PHOTO));
	Utils_GetSizeString(twlpUsedSize, Storage_GetUsedStorage(SYSTEM_MEDIATYPE_TWL_PHOTO));
	Utils_GetSizeString(twlpTotalSize, Storage_GetTotalStorage(SYSTEM_MEDIATYPE_TWL_PHOTO));

	Draw_Rect(0, 20, 400, 220, BACKGROUND_COLOUR);

	sdUsed = Storage_GetUsedStorage(SYSTEM_MEDIATYPE_SD);
	sdTotal = Storage_GetTotalStorage(SYSTEM_MEDIATYPE_SD);
	Draw_Rect(20, 105, 60, 10, MENU_INFO_TITLE_COLOUR);
	Draw_Rect(21, 106, 58, 8, BACKGROUND_COLOUR);
	Draw_Rect(21, 106, (((double)sdUsed / (double)sdTotal) * 58.00), 8, MENU_SELECTOR_COLOUR);
	Draw_Text(85, 50, 0.45f, MENU_INFO_DESC_COLOUR, "SD:");
	Menu_DrawItem(85, 71, "Free:", sdFreeSize);
	Menu_DrawItem(85, 87, "Used:", sdUsedSize);
	Menu_DrawItem(85, 103, "Total:", sdTotalSize);
	Draw_Image(drive_icon, 20, 40);

	ctrUsed = Storage_GetUsedStorage(SYSTEM_MEDIATYPE_CTR_NAND);
	ctrTotal = Storage_GetTotalStorage(SYSTEM_MEDIATYPE_CTR_NAND);
	Draw_Rect(220, 105, 60, 10, MENU_INFO_TITLE_COLOUR);
	Draw_Rect(221, 106, 58, 8, BACKGROUND_COLOUR);
	Draw_Rect(221, 106, (((double)ctrUsed / (double)ctrTotal) * 58.00), 8, MENU_SELECTOR_COLOUR);
	Draw_Text(285, 50, 0.45f, MENU_INFO_DESC_COLOUR, "CTR Nand:");
	Menu_DrawItem(285, 71, "Free:", ctrFreeSize);
	Menu_DrawItem(285, 87, "Used:", ctrUsedSize);
	Menu_DrawItem(285, 103, "Total:", ctrTotalSize);
	Draw_Image(drive_icon, 220, 40);

	twlUsed = Storage_GetUsedStorage(SYSTEM_MEDIATYPE_TWL_NAND);
	twlTotal = Storage_GetTotalStorage(SYSTEM_MEDIATYPE_TWL_NAND);
	Draw_Rect(20, 200, 60, 10, MENU_INFO_TITLE_COLOUR);
	Draw_Rect(21, 201, 58, 8, BACKGROUND_COLOUR);
	Draw_Rect(21, 201, (((double)twlUsed / (double)twlTotal) * 58.00), 8, MENU_SELECTOR_COLOUR);
	Draw_Text(85, 145, 0.45f, MENU_INFO_DESC_COLOUR, "TWL Nand:");
	Menu_DrawItem(85, 166, "Free:", twlFreeSize);
	Menu_DrawItem(85, 182, "Used:", twlUsedSize);
	Menu_DrawItem(85, 198, "Total:", twlTotalSize);
	Draw_Image(drive_icon, 20, 135);

	twlpUsed = Storage_GetUsedStorage(SYSTEM_MEDIATYPE_TWL_PHOTO);
	twlpTotal = Storage_GetTotalStorage(SYSTEM_MEDIATYPE_TWL_PHOTO);
	Draw_Rect(220, 200, 60, 10, MENU_INFO_TITLE_COLOUR);
	Draw_Rect(221, 201, 58, 8, BACKGROUND_COLOUR);
	Draw_Rect(221, 201, (((double)twlpUsed / (double)twlpTotal) * 58.00), 8, MENU_SELECTOR_COLOUR);
	Draw_Text(285, 145, 0.45f, MENU_INFO_DESC_COLOUR, "TWL Photo:");
	Menu_DrawItem(285, 166, "Free:", twlpFreeSize);
	Menu_DrawItem(285, 182, "Used:", twlpUsedSize);
	Menu_DrawItem(285, 198, "Total:", twlpTotalSize);
	Draw_Image(drive_icon, 220, 135);
}

static int touchButton(touchPosition *touch, int selection)
{
	if (touch->px >= 15 && touch->px <= 300 && touch->py >= 37 && touch->py <= 56)
		selection = 0;
	else if (touch->px >= 15 && touch->px <= 300 && touch->py >= 56 && touch->py <= 73)
		selection = 1;
	else if (touch->px >= 15 && touch->px <= 300 && touch->py >= 73 && touch->py <= 92)
		selection = 2;
	else if (touch->px >= 15 && touch->px <= 300 && touch->py >= 92 && touch->py <= 110)
		selection = 3;
	else if (touch->px >= 15 && touch->px <= 300 && touch->py >= 110 && touch->py <= 127)
		selection = 4;
	else if (touch->px >= 15 && touch->px <= 300 && touch->py >= 127 && touch->py <= 144)
		selection = 5;
	else if (touch->px >= 15 && touch->px <= 300 && touch->py >= 144 && touch->py <= 161)
		selection = 6;
	else if (touch->px >= 15 && touch->px <= 300 && touch->py >= 161 && touch->py <= 178)
		selection = 7;
	else if (touch->px >= 15 && touch->px <= 300 && touch->py >= 178 && touch->py <= 195)	
		selection = 8;
	else if (touch->px >= 15 && touch->px <= 300 && touch->py >= 195 && touch->py <= 212)	
		selection = 9;
	
	return selection;
}

void Menu_Main(void)
{
	int selection = 0;
	display_info = true;
	touchPosition touch;

	strcpy(kernel_version, Kernel_GetVersion(0));
	strcpy(firm_version, Kernel_GetVersion(1));
	strcpy(initial_version, Kernel_GetVersion(2));
	strcpy(system_version, Kernel_GetVersion(3));
	strcpy(nand_lfcs, System_GetNANDLocalFriendCodeSeed());

	sd_titles = Misc_TitleCount(MEDIATYPE_SD);
	nand_titles = Misc_TitleCount(MEDIATYPE_NAND);
	tickets = Misc_TicketCount();

	float instr_width = 0.0f, instr_width2 = 0.0f, instr_width3 = 0.0f, instr_height = 0.0f;
	Draw_GetTextSize(0.5f, &instr_width, &instr_height, "Press select to hide user-specific info.");
	Draw_GetTextSize(0.5f, &instr_width2, NULL, "Press L + R to capture a screenshot.");
	Draw_GetTextSize(0.5f, &instr_width3, NULL, "Press START + SELECT to use button tester.");

	while (aptMainLoop()) 
	{
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(RENDER_TOP, BACKGROUND_COLOUR);
		C2D_TargetClear(RENDER_BOTTOM, BACKGROUND_COLOUR);
		C2D_SceneBegin(RENDER_TOP);

		Draw_Rect(0, 0, 400, 20, STATUS_BAR_COLOUR);
		Draw_Textf(5, (20 - Draw_GetTextHeight(0.5f, "3DSident v0.0.0"))/2, 0.5f, BACKGROUND_COLOUR, "3DSident v%d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_MICRO);

		Draw_Image(banner, (400 - banner.subtex->width) / 2, ((82 - banner.subtex->height) / 2) + 20);

		switch(selection)
		{
			case 0:
				Menu_Kernel();
				break;
			case 1:
				Menu_System();
				break;
			case 2:
				Menu_Battery();
				break;
			case 3:
				Menu_NNID();
				break;
			case 4:
				Menu_Config();
				break;
			case 5:
				Menu_Hardware();
				break;
			case 6:
				Menu_WiFi();
				break;
			case 7:
				Menu_Storage();
				break;
			case 8:
				Menu_Misc();
				break;
			case 9:
				Draw_Text((400 - instr_width) / 2, (240 - instr_height) / 2, 0.5f, MENU_INFO_TITLE_COLOUR, "Press select to hide user-specific info.");
				Draw_Text((400 - instr_width2) / 2, ((240 - instr_height) / 2) + 18, 0.5f, MENU_INFO_TITLE_COLOUR, "Press L + R to capture a screenshot.");
				Draw_Text((400 - instr_width3) / 2, ((240 - instr_height) / 2) + 36, 0.5f, MENU_INFO_TITLE_COLOUR, "Press START + SELECT to use button tester.");
				break;
		}

		C2D_SceneBegin(RENDER_BOTTOM);

		Draw_Rect(15, 15, 290, 210, MENU_INFO_TITLE_COLOUR);
		Draw_Rect(16, 16, 288, 208, MENU_BAR_COLOUR);
		Draw_Rect(16, 16 + (DISTANCE_Y * selection), 288, 18, MENU_SELECTOR_COLOUR);

		Draw_Text(22, 18, 0.5f, selection == 0? ITEM_SELECTED_COLOUR : ITEM_COLOUR, "Kernel");
		Draw_Text(22, 38, 0.5f, selection == 1? ITEM_SELECTED_COLOUR : ITEM_COLOUR, "System");
		Draw_Text(22, 58, 0.5f, selection == 2? ITEM_SELECTED_COLOUR : ITEM_COLOUR, "Battery");
		Draw_Text(22, 78, 0.5f, selection == 3? ITEM_SELECTED_COLOUR : ITEM_COLOUR, "NNID");
		Draw_Text(22, 98, 0.5f, selection == 4? ITEM_SELECTED_COLOUR : ITEM_COLOUR, "Config");
		Draw_Text(22, 118, 0.5f, selection == 5? ITEM_SELECTED_COLOUR : ITEM_COLOUR, "Hardware");
		Draw_Text(22, 138, 0.5f, selection == 6? ITEM_SELECTED_COLOUR : ITEM_COLOUR, "WiFi");
		Draw_Text(22, 158, 0.5f, selection == 7? ITEM_SELECTED_COLOUR : ITEM_COLOUR, "Storage");
		Draw_Text(22, 178, 0.5f, selection == 8? ITEM_SELECTED_COLOUR : ITEM_COLOUR, "Miscellaneous");
		Draw_Text(22, 198, 0.5f, selection == 9? ITEM_SELECTED_COLOUR : ITEM_COLOUR, "Exit");

		Draw_EndFrame();

		hidScanInput();
		hidTouchRead(&touch);
		u32 kDown = hidKeysDown();
		u32 kHeld = hidKeysHeld();

		selection = touchButton(&touch, selection);

		if (kDown & KEY_DDOWN)
			selection++;
		else if (kDown & KEY_DUP)
			selection--;

		if (selection > MAX_ITEMS) 
			selection = 0;
		if (selection < 0) 
			selection = MAX_ITEMS;

		if (kDown & KEY_SELECT)
			display_info = !display_info;

		if (((kHeld & KEY_START) && (kDown & KEY_SELECT)) || ((kHeld & KEY_SELECT) && (kDown & KEY_START)))
			MENU_STATE_CONTROLS = true;

		Menu_Controls();

		if (((kHeld & KEY_L) && (kDown & KEY_R)) || ((kHeld & KEY_R) && (kDown & KEY_L)))
			Screenshot_Capture();

		if (kDown & KEY_A)
		{
			if (selection == 9)
				longjmp(exitJmp, 1);
		}
	}
}
