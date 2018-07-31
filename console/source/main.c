#include <malloc.h>
#include <stdio.h>

#include <3ds.h>

#include "ac.h"
#include "actu.h"
#include "fs.h"
#include "hardware.h"
#include "kernel.h"
#include "misc.h"
#include "screenshot.h"
#include "storage.h"
#include "system.h"
#include "utils.h"
#include "wifi.h"

#define ANY_KEY (KEY_TOUCH | KEY_A | KEY_B | KEY_X | KEY_Y | KEY_START | \
		KEY_UP | KEY_DOWN | KEY_LEFT | KEY_RIGHT  | KEY_ZL | KEY_ZR | \
		KEY_CSTICK_UP | KEY_CSTICK_DOWN | KEY_CSTICK_LEFT | KEY_CSTICK_RIGHT)

static u32 cpu_time_limit = 0;

void Init_Services(void)
{
	gfxInitDefault();
	aciInit();
	
	// Check if user is running from *hax.
	//if (envIsHomebrew() && (R_FAILED(actInit())))
	//	isHomebrew = envIsHomebrew();
	
	actInit();
	
	amAppInit();
	amInit();
	cfguInit();
	dspInit();
	mcuHwcInit();
	ptmuInit();
	socInit((u32*)memalign(0x1000, 0x10000), 0x10000);

	FS_OpenArchive(&archive, ARCHIVE_SDMC);
	
	if (Utils_IsN3DS())
		osSetSpeedupEnable(true);

	APT_GetAppCpuTimeLimit(&cpu_time_limit);
	APT_SetAppCpuTimeLimit(30);
}

void Term_Services(void)
{
	if (cpu_time_limit != UINT32_MAX)
		APT_SetAppCpuTimeLimit(cpu_time_limit);

	if (Utils_IsN3DS())
		osSetSpeedupEnable(false);
	
	FS_CloseArchive(archive);
	
	socExit();
	ptmuExit();
	mcuHwcExit();
	dspExit();
	cfguExit();
	amExit();
	actExit();
	acExit();
	aciExit();
	gfxExit();
}

int main(int argc, char *argv[])
{
	Init_Services();
	
	//=====================================================================//
	//------------------------Variable Declaration-------------------------//
	//=====================================================================//
	
	double wifiPercent = 0, volPercent = 0, _3dSliderPercent = 0;
	u32 ip = 0;
	unsigned int principalID = 0;
	u8 battery_percent = 0, battery_status = 0, battery_volt = 0, volume = 0, fw_ver_high = 0, fw_ver_low = 0;
	bool is_connected = false;
	char sdFreeSize[16], sdTotalSize[16], ctrFreeSize[16], ctrTotalSize[16], country[0x3], name[0x16], nnid[0x11], timeZone[0x41];
	AccountDataBlock accountDataBlock;
	bool displayInfo = true; // By default nothing is hidden.

	hidScanInput();
	u32 kHeld = hidKeysHeld();
		
	if (kHeld & KEY_SELECT)
		displayInfo = false; // Holding select on boot hides user specific details.
	
	consoleInit(GFX_BOTTOM, NULL);
	
	//=====================================================================//
	//------------------------------MISC Info (continued)------------------//
	//=====================================================================//
	
	printf("\x1b[36;1m*\x1b[0m Installed titles: SD: \x1b[36;1m%lu\x1b[0m  (NAND: \x1b[36;1m%lu\x1b[0m)\n", Misc_TitleCount(MEDIATYPE_SD), Misc_TitleCount(MEDIATYPE_NAND));
	
	ip = gethostid();
	if (displayInfo)
		printf("\x1b[36;1m*\x1b[0m IP: \x1b[36;1m%lu.%lu.%lu.%lu\x1b[0m     \n\n", ip & 0xFF, (ip>>8)&0xFF, (ip>>16)&0xFF, (ip>>24)&0xFF);
	else 
		printf("\x1b[36;1m*\x1b[0m IP: \x1b[36;1m%s\x1b[0m     \n\n", NULL);

	//=====================================================================//
	//------------------------------NNID Info------------------------------//
	//=====================================================================//
	
	ACTU_GetAccountDataBlock(&principalID, 0x4, 0xC); // First check if principal ID exists then display NNID info.
	ACTU_GetAccountDataBlock((u8*)&accountDataBlock, 0xA0, 0x11);
	
	if (principalID != 0)
	{
		if (R_SUCCEEDED(ACTU_GetAccountDataBlock(nnid, 0x11, 0x8)))
			printf("\x1b[35;1m*\x1b[0m NNID: \x1b[35;1m%s\x1b[0m\n", displayInfo? nnid : NULL);

		printf("\x1b[35;1m*\x1b[0m Principal ID: \x1b[35;1m%u\x1b[0m\n", displayInfo? principalID : 0);

		printf("\x1b[35;1m*\x1b[0m Persistent ID: \x1b[35;1m%u\x1b[0m\n", displayInfo? (unsigned int)accountDataBlock.persistentID : 0);

		printf("\x1b[35;1m*\x1b[0m Transferable ID: \x1b[35;1m%llu\x1b[0m\n", displayInfo? accountDataBlock.transferableID : 0);
	
		if (R_SUCCEEDED(ACTU_GetAccountDataBlock(country, 0x3, 0xB)))
			printf("\x1b[35;1m*\x1b[0m Country: \x1b[35;1m%s\x1b[0m\n", displayInfo? country : NULL);
	
		if (R_SUCCEEDED(ACTU_GetAccountDataBlock(timeZone, 0x41, 0x1E)))
			printf("\x1b[35;1m*\x1b[0m Time Zone: \x1b[35;1m%s\x1b[0m\n\n", displayInfo? timeZone : NULL);
	}
	
	//=====================================================================//
	//------------------------------WIFI Info------------------------------//
	//=====================================================================//
	
	if (R_SUCCEEDED(ACI_LoadWiFiSlot(0)))
	{
		char ssid[0x20], passphrase[0x40];
		
		if (R_SUCCEEDED(ACI_GetSSID(ssid)))
			printf("\x1b[32;1m*\x1b[0m WiFi 1 SSID: \x1b[32;1m%s\x1b[0m\n", ssid);

		if (R_SUCCEEDED(ACI_GetPassphrase(passphrase)))
			printf("\x1b[32;1m*\x1b[0m WiFi 1 pass: \x1b[32;1m%s\x1b[0m\n", displayInfo? passphrase : NULL);

		printf("\x1b[32;1m*\x1b[0m WiFi 1 security: \x1b[32;1m%s\x1b[0m\n\n", displayInfo? WiFi_GetSecurityMode() : NULL);
	}
	
	if (R_SUCCEEDED(ACI_LoadWiFiSlot(1)))
	{
		char ssid[0x20], passphrase[0x40];
		
		if (R_SUCCEEDED(ACI_GetSSID(ssid)))
			printf("\x1b[32;1m*\x1b[0m WiFi 2 SSID: \x1b[32;1m%s\x1b[0m\n", ssid);

		if (R_SUCCEEDED(ACI_GetPassphrase(passphrase)))
			printf("\x1b[32;1m*\x1b[0m WiFi 2 pass: \x1b[32;1m%s\x1b[0m\n", displayInfo? passphrase : NULL);

		printf("\x1b[32;1m*\x1b[0m WiFi 2 security: \x1b[32;1m%s\x1b[0m\n\n", displayInfo? WiFi_GetSecurityMode() : NULL);
	}
	
	if (R_SUCCEEDED(ACI_LoadWiFiSlot(2)))
	{
		char ssid[0x20], passphrase[0x40];
		
		if (R_SUCCEEDED(ACI_GetSSID(ssid)))
			printf("\x1b[32;1m*\x1b[0m WiFi 3 SSID: \x1b[32;1m%s\x1b[0m\n", ssid);

		if (R_SUCCEEDED(ACI_GetPassphrase(passphrase)))
			printf("\x1b[32;1m*\x1b[0m WiFi 3 pass: \x1b[32;1m%s\x1b[0m\n", displayInfo? passphrase : NULL);

		printf("\x1b[32;1m*\x1b[0m WiFi 3 security: \x1b[32;1m%s\x1b[0m\n\n", displayInfo? WiFi_GetSecurityMode() : NULL);
	}
	
	printf("\x1b[32;1m> Press any key to exit =)\x1b[0m");
	
	consoleInit(GFX_TOP, NULL);

	printf("\x1b[1;1H"); //Move the cursor to the top left corner of the screen
	printf("\x1b[32;1m3DSident 0.7.9\x1b[0m\n\n");

	//=====================================================================//
	//------------------------------Firm Info------------------------------//
	//=====================================================================//
			
	printf("\x1b[33;1m*\x1b[0m Kernel version: \x1b[33;1m%s\n", Kernel_GetVersion(0));
	printf("\x1b[33;1m*\x1b[0m Firm version: \x1b[33;1m%s\n", Kernel_GetVersion(1));
	printf("\x1b[33;1m*\x1b[0m System version: \x1b[33;1m%s\n\n", Kernel_GetVersion(3));
	printf("\x1b[33;1m*\x1b[0m Initial System version: \x1b[33;1m%s\n\n", Kernel_GetVersion(2));
		
	//=====================================================================//
	//-----------------------------System Info-----------------------------//
	//=====================================================================//
	
	printf("\x1b[31;1m*\x1b[0m Model: \x1b[31;1m%s\x1b[0m (\x1b[31;1m%s\x1b[0m - \x1b[31;1m%s\x1b[0m) \n\x1b[0m", System_GetModel(), System_GetRunningHW(), System_GetRegion());
	printf("\x1b[31;1m*\x1b[0m Screen type: \x1b[31;1m %s \n\x1b[0m", System_GetScreenType());
	printf("\x1b[31;1m*\x1b[0m Language: \x1b[31;1m%s\x1b[0m \n", System_GetLang());
	printf("\x1b[31;1m*\x1b[0m Device ID: \x1b[31;1m%lu \n", displayInfo? Kernel_GetDeviceId() : 0);
	printf("\x1b[31;1m*\x1b[0m ECS Device ID: \x1b[31;1m%llu \n", displayInfo? System_GetSoapId() : 0);
	printf("\x1b[31;1m*\x1b[0m Local friend code seed: \x1b[31;1m%010llX\x1b[0m \n", displayInfo? System_GetLocalFriendCodeSeed() : 0);	
	printf("\x1b[31;1m*\x1b[0m MAC Address: \x1b[31;1m%s\x1b[0m \n", displayInfo? System_GetMacAddress() : 0);
	printf("\x1b[31;1m*\x1b[0m Serial number: \x1b[31;1m%s\x1b[0m \n", displayInfo? System_GetSerialNumber() : 0);
	printf("\x1b[31;1m*\x1b[0m SDMC CID: \x1b[31;1m%s\x1b[0m \n", displayInfo? Kernel_GetSDMCCID() : 0);
	printf("\x1b[31;1m*\x1b[0m NAND CID: \x1b[31;1m%s\x1b[0m \n\n", displayInfo? Kernel_GetNANDCID() : 0);
		
	while (aptMainLoop())
	{
		//=====================================================================//
		//----------------------------Battery Info-----------------------------//
		//=====================================================================//
		
		printf("\x1b[19;0H");
		if ((R_SUCCEEDED(MCUHWC_GetBatteryLevel(&battery_percent))) || R_SUCCEEDED(PTMU_GetBatteryChargeState(&battery_status)))
			printf("\x1b[34;1m*\x1b[0m Battery percentage: \x1b[34;1m%3d%%\x1b[0m (\x1b[34;1m%s\x1b[0m)     \n", battery_percent, battery_status);

		printf("\x1b[20;0H");
		if (R_SUCCEEDED(MCUHWC_GetBatteryVoltage(&battery_volt)))
			printf("\x1b[34;1m*\x1b[0m Battery voltage: \x1b[34;1m%d\x1b[0m (\x1b[34;1m%.1f V\x1b[0m)    \n", battery_volt, 5.0 * ((double)battery_volt / 256.0));//,(Estimated: %0.1lf V) estimatedVolt);
		
		printf("\x1b[21;0H");
		if (R_SUCCEEDED(PTMU_GetAdapterState(&is_connected)))
			printf("\x1b[34;1m*\x1b[0m Adapter state: \x1b[34;1m%s\x1b[0m\n", is_connected? "connected   " : "disconnected");
		
		printf("\x1b[22;0H");
		if ((R_SUCCEEDED(MCUHWC_GetFwVerHigh(&fw_ver_high))) && (R_SUCCEEDED(MCUHWC_GetFwVerLow(&fw_ver_low))))
			printf("\x1b[34;1m*\x1b[0m MCU firmware: \x1b[34;1m%u.%u\x1b[0m\n\n", (fw_ver_high - 16), fw_ver_low);
		
		//=====================================================================//
		//------------------------------Misc Info------------------------------//
		//=====================================================================//
		
		printf("\x1b[24;0H");
		Utils_GetSizeString(sdFreeSize, Storage_GetFreeStorage(SYSTEM_MEDIATYPE_SD));
		Utils_GetSizeString(sdTotalSize, Storage_GetTotalStorage(SYSTEM_MEDIATYPE_SD));
		printf("\x1b[36;1m*\x1b[0m SD Size: \x1b[36;1m%s\x1b[0m / \x1b[36;1m%s\x1b[0m \n", sdFreeSize, sdTotalSize);

		printf("\x1b[25;0H");
		Utils_GetSizeString(ctrFreeSize, Storage_GetFreeStorage(SYSTEM_MEDIATYPE_CTR_NAND));
		Utils_GetSizeString(ctrTotalSize, Storage_GetTotalStorage(SYSTEM_MEDIATYPE_CTR_NAND));
		printf("\x1b[36;1m*\x1b[0m CTR Size: \x1b[36;1m%s\x1b[0m / \x1b[36;1m%s\x1b[0m \n", ctrFreeSize, ctrTotalSize);

		printf("\x1b[26;0H");
		printf("\x1b[36;1m*\x1b[0m Brightness level: \x1b[36;1m%s\x1b[0m    \n", Hardware_GetBrightness(GSPLCD_SCREEN_TOP));
		
		printf("\x1b[27;0H");
		wifiPercent = (osGetWifiStrength() * 33.3333333333);
		printf("\x1b[36;1m*\x1b[0m WiFi signal strength: \x1b[36;1m%d\x1b[0m  (\x1b[36;1m%.0lf%%\x1b[0m)    \n", osGetWifiStrength(), wifiPercent);

		printf("\x1b[28;0H");
		if (R_SUCCEEDED(HIDUSER_GetSoundVolume(&volume)))
		{
			volPercent = (volume * 1.5873015873);
			printf("\x1b[36;1m*\x1b[0m Volume slider state: \x1b[36;1m%d\x1b[0m  (\x1b[36;1m%.0lf%%\x1b[0m)    \n", volume, volPercent);
		}

		printf("\x1b[29;0H");
		_3dSliderPercent = (osGet3DSliderState() * 100.0);
		printf("\x1b[36;1m*\x1b[0m 3D slider state: \x1b[36;1m%.1lf\x1b[0m  (\x1b[36;1m%.0lf%%\x1b[0m)    \n", osGet3DSliderState(), _3dSliderPercent);
		
		gspWaitForVBlank();
		hidScanInput();
		u32 kDown = hidKeysDown();
		u32 kHeld = hidKeysHeld();
		
		if (((kHeld & KEY_L) && (kDown & KEY_R)) || ((kHeld & KEY_R) && (kDown & KEY_L)))
			Screenshot_Capture();
		
		else if (kDown & ANY_KEY)
			break;
		
		gfxFlushBuffers();
		gfxSwapBuffers();
	}
	
	Term_Services();
	return 0;
}
