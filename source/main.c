#include <3ds.h>

#include <malloc.h>

#include "ac.h"
#include "actu.h"
#include "fs.h"
#include "hardware.h"
#include "kernel.h"
#include "mcu.h"
#include "misc.h"
#include "power.h"
#include "screenshot.h"
#include "storage.h"
#include "system.h"
#include "utils.h"

#define SDK(a, b, c, d) ((a<<24) | (b<<16) | (c<<8) | d)

void initServices()
{
	gfxInitDefault();
	cfguInit();
	ptmuInit();
	mcuInit();
	amInit();
	amAppInit();
	aptInit();
	hidInit();
	aciInit();
	actInit();
	socInit((u32*)memalign(0x1000, 0x10000), 0x10000);
	
	fsInit();
	sdmcInit();
	openArchive(&fsArchive, ARCHIVE_SDMC);
	
	if (isN3DS())
		osSetSpeedupEnable(true);
}

void termServices()
{
	if (isN3DS())
		osSetSpeedupEnable(false);
	
	closeArchive(fsArchive);
	sdmcExit();
	fsExit();
	
	socExit();
	actExit();
	hidExit();
	aptExit();
	acExit();
	aciExit();
	amExit();
	mcuExit();
	ptmuExit();
	cfguExit();
	gfxExit();
}

int main(int argc, char *argv[])
{
	initServices();
	
	//=====================================================================//
	//------------------------Variable Declaration-------------------------//
	//=====================================================================//
	
	double wifiPercent, volPercent, _3dSliderPercent;
	unsigned long long homemenuID = 0;
	u32 ip;
	unsigned int principalID = 0;
	u8 batteryPercent = 0, batteryVolt = 0, volume = 0, mcuFwMajor = 0, mcuFwMinor = 0;
	bool isConnected = false;
	char sdFreeSize[16], sdTotalSize[16], ctrFreeSize[16], ctrTotalSize[16], country[0x3], name[0x16], nnid[0x11], timeZone[0x41];
	AccountDataBlock accountDataBlock;
	MiiData miiData;
	
	consoleInit(GFX_BOTTOM, NULL);
	
	//=====================================================================//
	//------------------------------MISC Info (continued)------------------//
	//=====================================================================//
	
	printf("\x1b[36;1m*\x1b[0m Installed titles: SD: \x1b[36;1m%lu\x1b[0m  (NAND: \x1b[36;1m%lu\x1b[0m)\n", titleCount(MEDIATYPE_SD), titleCount(MEDIATYPE_NAND));
	
	ip = gethostid();
	printf("\x1b[36;1m*\x1b[0m IP: \x1b[36;1m%lu.%lu.%lu.%lu\x1b[0m     \n\n", ip & 0xFF, (ip>>8)&0xFF, (ip>>16)&0xFF, (ip>>24)&0xFF);
	
	//=====================================================================//
	//------------------------------NNID Info------------------------------//
	//=====================================================================//
	
	ACTU_GetAccountDataBlock(&principalID, 0x4, 0xC); // First check if principal ID exists then display NNID info.
	
	ACTU_GetAccountDataBlock((u8*)&accountDataBlock, 0xA0, 0x11);
	ACTU_GetAccountDataBlock((u8*)&miiData, 0x60, 0x7);
	
	if (principalID != 0)
	{
		if (R_SUCCEEDED(ACTU_GetAccountDataBlock(nnid, 0x11, 0x8)))
			printf("\x1b[35;1m*\x1b[0m NNID: \x1b[35;1m%s\x1b[0m\n", nnid);
		
		u16_to_u8(name, accountDataBlock.miiName, 0x16);
		printf("\x1b[35;1m*\x1b[0m Mii name: \x1b[35;1m%s\x1b[0m (\x1b[35;1m%u\x1b[0m)\n", name, (unsigned int)miiData.miiID);

		printf("\x1b[35;1m*\x1b[0m Principal ID: \x1b[35;1m%u\x1b[0m\n", principalID);

		printf("\x1b[35;1m*\x1b[0m Persistent ID: \x1b[35;1m%u\x1b[0m\n", (unsigned int)accountDataBlock.persistentID);

		printf("\x1b[35;1m*\x1b[0m Transferable ID: \x1b[35;1m%llu\x1b[0m\n", accountDataBlock.transferableID);
	
		if (R_SUCCEEDED(ACTU_GetAccountDataBlock(country, 0x3, 0xB)))
			printf("\x1b[35;1m*\x1b[0m Country: \x1b[35;1m%s\x1b[0m\n", country);
	
		if (R_SUCCEEDED(ACTU_GetAccountDataBlock(timeZone, 0x41, 0x1E)))
			printf("\x1b[35;1m*\x1b[0m Time Zone: \x1b[35;1m%s\x1b[0m\n\n", timeZone);
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
			printf("\x1b[32;1m*\x1b[0m WiFi 1 pass: \x1b[32;1m%s\x1b[0m\n", passphrase);

		printf("\x1b[32;1m*\x1b[0m WiFi 1 security: \x1b[32;1m%s\x1b[0m\n\n", getSecurityMode());
	}
	
	if (R_SUCCEEDED(ACI_LoadWiFiSlot(1)))
	{
		char ssid[0x20], passphrase[0x40];
		
		if (R_SUCCEEDED(ACI_GetSSID(ssid)))
			printf("\x1b[32;1m*\x1b[0m WiFi 2 SSID: \x1b[32;1m%s\x1b[0m\n", ssid);

		if (R_SUCCEEDED(ACI_GetPassphrase(passphrase)))
			printf("\x1b[32;1m*\x1b[0m WiFi 2 pass: \x1b[32;1m%s\x1b[0m\n", passphrase);

		printf("\x1b[32;1m*\x1b[0m WiFi 2 security: \x1b[32;1m%s\x1b[0m\n\n", getSecurityMode());
	}
	
	if (R_SUCCEEDED(ACI_LoadWiFiSlot(2)))
	{
		char ssid[0x20], passphrase[0x40];
		
		if (R_SUCCEEDED(ACI_GetSSID(ssid)))
			printf("\x1b[32;1m*\x1b[0m WiFi 3 SSID: \x1b[32;1m%s\x1b[0m\n", ssid);

		if (R_SUCCEEDED(ACI_GetPassphrase(passphrase)))
			printf("\x1b[32;1m*\x1b[0m WiFi 3 pass: \x1b[32;1m%s\x1b[0m\n", passphrase);

		printf("\x1b[32;1m*\x1b[0m WiFi 3 security: \x1b[32;1m%s\x1b[0m\n\n", getSecurityMode());
	}
	
	printf("\x1b[32;1m> Press any key to exit =)\x1b[0m");
	
	consoleInit(GFX_TOP, NULL);

	printf("\x1b[1;1H"); //Move the cursor to the top left corner of the screen
	printf("\x1b[32;1m3DSident 0.7.7\x1b[0m\n\n");

	//=====================================================================//
	//------------------------------Firm Info------------------------------//
	//=====================================================================//
			
	printf("\x1b[33;1m*\x1b[0m Kernel version: \x1b[33;1m%s\n", getVersion(0));
	
	printf("\x1b[33;1m*\x1b[0m Firm version: \x1b[33;1m%s\n", getVersion(1));
	
	printf("\x1b[33;1m*\x1b[0m System version: \x1b[33;1m%s\n\n", getVersion(2));
		
	//=====================================================================//
	//-----------------------------System Info-----------------------------//
	//=====================================================================//
	
	printf("\x1b[31;1m*\x1b[0m Model: \x1b[31;1m%s\x1b[0m (\x1b[31;1m%s\x1b[0m - \x1b[31;1m%s\x1b[0m) \n\x1b[0m", getModel(), getRunningHW(), getRegion());
	printf("\x1b[31;1m*\x1b[0m Screen type: \x1b[31;1m %s \n\x1b[0m", getScreenType());
	printf("\x1b[31;1m*\x1b[0m Language: \x1b[31;1m%s\x1b[0m \n", getLang());

	printf("\x1b[31;1m*\x1b[0m Device ID: \x1b[31;1m%lu \n", getDeviceId());
	printf("\x1b[31;1m*\x1b[0m ECS Device ID: \x1b[31;1m%llu \n", getSoapId());
	printf("\x1b[31;1m*\x1b[0m Local friend code seed: \x1b[31;1m%010llX\x1b[0m \n", getLocalFriendCodeSeed());	
	printf("\x1b[31;1m*\x1b[0m MAC Address: \x1b[31;1m%s\x1b[0m \n", getMacAddress());
	printf("\x1b[31;1m*\x1b[0m Serial number: \x1b[31;1m%s\x1b[0m \n", getSerialNumber());

	printf("\x1b[31;1m*\x1b[0m SDMC CID: \x1b[31;1m%s\x1b[0m \n", getSdmcCid());

	printf("\x1b[31;1m*\x1b[0m NAND CID: \x1b[31;1m%s\x1b[0m \n", getNandCid());
			
	if (R_SUCCEEDED(APT_GetAppletInfo(APPID_HOMEMENU, &homemenuID, NULL, NULL, NULL, NULL)))
		printf("\x1b[31;1m*\x1b[0m Homemenu ID: \x1b[31;1m%016llX\x1b[0m \n\n", homemenuID);
		
	while (aptMainLoop())
	{
		//=====================================================================//
		//----------------------------Battery Info-----------------------------//
		//=====================================================================//
		
		printf("\x1b[19;0H");
		if (R_SUCCEEDED(MCU_GetBatteryLevel(&batteryPercent)))
			printf("\x1b[34;1m*\x1b[0m Battery percentage: \x1b[34;1m%3d%%\x1b[0m (\x1b[34;1m%s\x1b[0m)     \n", batteryPercent, batteryStatus());

		printf("\x1b[20;0H");
		if (R_SUCCEEDED(MCU_GetBatteryVoltage(&batteryVolt)))
			printf("\x1b[34;1m*\x1b[0m Battery voltage: \x1b[34;1m%d\x1b[0m (\x1b[34;1m%.1f V\x1b[0m)    \n", batteryVolt, 5.0 * ((double)batteryVolt / 256.0));//,(Estimated: %0.1lf V) estimatedVolt);
		
		printf("\x1b[21;0H");
		if (R_SUCCEEDED(PTMU_GetAdapterState(&isConnected)))
			printf("\x1b[34;1m*\x1b[0m Adapter state: \x1b[34;1m%s\x1b[0m\n", isConnected? "connected   " : "disconnected");
		
		printf("\x1b[22;0H");
		if ((R_SUCCEEDED(MCU_GetFwVerHigh(&mcuFwMajor))) && (R_SUCCEEDED(MCU_GetFwVerLow(&mcuFwMinor))))
			printf("\x1b[34;1m*\x1b[0m MCU firmware: \x1b[34;1m%u.%u\x1b[0m\n\n", (mcuFwMajor - 16), mcuFwMinor);
		
		//=====================================================================//
		//------------------------------Misc Info------------------------------//
		//=====================================================================//
		
		printf("\x1b[24;0H");
		getSizeString(sdFreeSize, getFreeStorage(SYSTEM_MEDIATYPE_SD));
		getSizeString(sdTotalSize, getTotalStorage(SYSTEM_MEDIATYPE_SD));
		printf("\x1b[36;1m*\x1b[0m SD Size: \x1b[36;1m%s\x1b[0m / \x1b[36;1m%s\x1b[0m \n", sdFreeSize, sdTotalSize);

		printf("\x1b[25;0H");
		getSizeString(ctrFreeSize, getFreeStorage(SYSTEM_MEDIATYPE_CTR_NAND));
		getSizeString(ctrTotalSize, getTotalStorage(SYSTEM_MEDIATYPE_CTR_NAND));
		printf("\x1b[36;1m*\x1b[0m CTR Size: \x1b[36;1m%s\x1b[0m / \x1b[36;1m%s\x1b[0m \n", ctrFreeSize, ctrTotalSize);

		printf("\x1b[26;0H");
		printf("\x1b[36;1m*\x1b[0m Brightness: \x1b[36;1m%s\x1b[0m    \n", getBrightness(1));
		
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
		u32 kHeld = hidKeysHeld();
		
		if ((kHeld & KEY_L) && (kHeld & KEY_R))
			captureScreenshot();
		
		else if (hidKeysDown() & ANY_KEY)
			break;
		
		gfxFlushBuffers();
		gfxSwapBuffers();
	}
	
	termServices();
	return 0;
}
