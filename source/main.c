#include <3ds.h>

#include "actu.h"
#include "cfgs.h"
#include "fs.h"
#include "mcu.h"
#include "misc.h"
#include "power.h"
#include "screenshot.h"
#include "system.h"
#include "utils.h"
#include "wifi.h"

#define SDK(a, b, c, d) ((a<<24) | (b<<16) | (c<<8) | d)

void initServices()
{
	gfxInitDefault();
	cfguInit();
	cfgsInit();
	fsInit();
	sdmcInit();
	openArchive(ARCHIVE_SDMC);
	ptmuInit();
	mcuInit();
	amInit();
	amAppInit();
	psInit();
	aptInit();
	hidInit();
	actInit(SDK(11, 2, 0, 200), 0x20000);
	socInit((u32*)memalign(0x1000, 0x10000), 0x10000);
}

void termServices()
{
	socExit();
	actExit();
	hidExit();
	aptExit();
	psExit();
	acExit();
	amExit();
	mcuExit();
	ptmuExit();
	closeArchive();
	sdmcExit();
	fsExit();
	cfgsExit();
	cfguExit();
	gfxExit();
}

int main(int argc, char *argv[])
{
	initServices();
	
	//=====================================================================//
	//------------------------Variable Declaration-------------------------//
	//=====================================================================//
	
	char *str_ver = (char *)malloc(sizeof(char) * 255), *str_sysver = (char *)malloc(sizeof(char) * 255);
	double wifiPercent, volPercent, _3dSliderPercent;
	unsigned long long transferableID = 0, homemenuID = 0;
	u32 os_ver = osGetKernelVersion(), firm_ver = osGetKernelVersion(), ip;
	unsigned int principalID = 0, persistentID = 0;
	u16 info[0x16];
	u8 buf[16], batteryPercent = 0, batteryVolt = 0, volume = 0, mcuFwMajor = 0, mcuFwMinor = 0;
	bool isConnected = false;
	OS_VersionBin *nver = (OS_VersionBin *)malloc(sizeof(OS_VersionBin)), *cver = (OS_VersionBin *)malloc(sizeof(OS_VersionBin));
	char sdFreeSize[16], sdTotalSize[16], ctrFreeSize[16], ctrTotalSize[16], name[0x16];
	Result ret = 0, wifiRet = 0;
	wifiBlock slotData;
	
	consoleInit(GFX_BOTTOM, NULL);
	
	//=====================================================================//
	//------------------------------MISC Info (continued)------------------//
	//=====================================================================//
	
	printf("\x1b[36;1m*\x1b[0m Brightness: \x1b[36;1m%s\x1b[0m    \n", getBrightness(1));
	
	ip = gethostid();
	printf("\x1b[36;1m*\x1b[0m IP: \x1b[36;1m%lu.%lu.%lu.%lu\x1b[0m     \n\n", ip & 0xFF, (ip>>8)&0xFF, (ip>>16)&0xFF, (ip>>24)&0xFF);
	
	//=====================================================================//
	//------------------------------NNID Info------------------------------//
	//=====================================================================//
	
	ACTU_GetAccountDataBlock(&principalID, 0x4, 0xC); // First check if principal ID exists then display NNID info.
	
	if (principalID != 0)
	{
		printf("\x1b[35;1m*\x1b[0m NNID: \x1b[35;1m%s\x1b[0m\n", getNNIDInfo(0x11, 0x8));
	
		ACTU_GetAccountDataBlock(info, 0x16, 0x1B);
		utf2ascii(name, info);
		printf("\x1b[35;1m*\x1b[0m Mii name: \x1b[35;1m%s\x1b[0m\n", name);

		printf("\x1b[35;1m*\x1b[0m Principal ID: \x1b[35;1m%u\x1b[0m\n", principalID);
	
		ACTU_GetAccountDataBlock(&persistentID, 0x4, 0x5);
		printf("\x1b[35;1m*\x1b[0m Persistent ID: \x1b[35;1m%u\x1b[0m\n", persistentID);
	
		ACTU_GetAccountDataBlock(&transferableID, 0x8, 0x6);
		printf("\x1b[35;1m*\x1b[0m Transferable ID: \x1b[35;1m%llu\x1b[0m\n", transferableID);
	
		printf("\x1b[35;1m*\x1b[0m Country: \x1b[35;1m%s\x1b[0m\n", getNNIDInfo(0x3, 0xB));
	
		printf("\x1b[35;1m*\x1b[0m Time Zone: \x1b[35;1m%s\x1b[0m\n\n", getNNIDInfo(0x41, 0x1E));
	}
	
	//=====================================================================//
	//------------------------------WIFI Info------------------------------//
	//=====================================================================//
	
	wifiRet = CFG_GetConfigInfoBlk8(CFG_WIFI_SLOT_SIZE, CFG_WIFI_BLKID + 0, (u8*)&slotData);
	if ((!wifiRet) && (slotData.exists))
	{
		if (slotData.network.use) 
			printf("\x1b[32;1m*\x1b[0m WiFi Slot 1: \x1b[32;1m%s\x1b[0m (\x1b[32;1m%s\x1b[0m)\n", slotData.network.SSID, slotData.network.password);
		else if (slotData.network_WPS.use) 
			printf("\x1b[32;1m*\x1b[0m WiFi Slot 1: \x1b[32;1m%s\x1b[0m (\x1b[32;1m%s\x1b[0m)\n", slotData.network_WPS.SSID, slotData.network_WPS.password);
	}
	
	wifiRet = CFG_GetConfigInfoBlk8(CFG_WIFI_SLOT_SIZE, CFG_WIFI_BLKID + 1, (u8*)&slotData);
	if ((!wifiRet) && (slotData.exists))
	{
		if (slotData.network.use) 
			printf("\x1b[32;1m*\x1b[0m WiFi Slot 2: \x1b[32;1m%s\x1b[0m (\x1b[32;1m%s\x1b[0m)\n", slotData.network.SSID, slotData.network.password);
		else if (slotData.network_WPS.use) 
			printf("\x1b[32;1m*\x1b[0m WiFi Slot 2: \x1b[32;1m%s\x1b[0m (\x1b[32;1m%s\x1b[0m)\n", slotData.network_WPS.SSID, slotData.network_WPS.password);
	}
	
	wifiRet = CFG_GetConfigInfoBlk8(CFG_WIFI_SLOT_SIZE, CFG_WIFI_BLKID + 2, (u8*)&slotData);
	if ((!wifiRet) && (slotData.exists))
	{
		if (slotData.network.use) 
			printf("\x1b[32;1m*\x1b[0m WiFi Slot 3: \x1b[32;1m%s\x1b[0m (\x1b[32;1m%s\x1b[0m)\n", slotData.network.SSID, slotData.network.password);
		else if (slotData.network_WPS.use) 
			printf("\x1b[32;1m*\x1b[0m WiFi Slot 3: \x1b[32;1m%s\x1b[0m (\x1b[32;1m%s\x1b[0m)\n", slotData.network_WPS.SSID, slotData.network_WPS.password);
	}
	
	printf("\n\x1b[32;1m> Press any key to exit =)\x1b[0m");
	
	consoleInit(GFX_TOP, NULL);

	printf("\x1b[1;1H"); //Move the cursor to the top left corner of the screen
	printf("\x1b[32;1m3DSident 0.7.6\x1b[0m\n\n");

	//=====================================================================//
	//------------------------------Firm Info------------------------------//
	//=====================================================================//
		
	snprintf(str_ver, 255, "\x1b[33;1m*\x1b[0m Kernel version: \x1b[33;1m%lu.%lu-%lu\n*\x1b[0m FIRM version is: \x1b[33;1m%lu.%lu-%lu\x1b[0m \n",
			GET_VERSION_MAJOR(os_ver),
			GET_VERSION_MINOR(os_ver),
			GET_VERSION_REVISION(os_ver),
			GET_VERSION_MAJOR(firm_ver),
			GET_VERSION_MINOR(firm_ver),
			GET_VERSION_REVISION(firm_ver)
			);

	printf(str_ver);

	memset(nver, 0, sizeof(OS_VersionBin));
	memset(cver, 0, sizeof(OS_VersionBin));
	ret = osGetSystemVersionData(nver, cver);

	if (ret)
		printf("\x1b[33;1m*\x1b[0m System version: \x1b[33;1m0x%08liX\x1b[0m\n\n", ret);

	snprintf(str_sysver, 100, "\x1b[33;1m*\x1b[0m System version: \x1b[33;1m%d.%d.%d-%d%c\x1b[0m\n\n",
			cver->mainver,
			cver->minor,
			cver->build,
			nver->mainver,
			getFirmRegion()
			);

	if (!ret)
		printf(str_sysver);
		
	//=====================================================================//
	//-----------------------------System Info-----------------------------//
	//=====================================================================//
	
	printf("\x1b[31;1m*\x1b[0m Model: \x1b[31;1m%s\x1b[0m (\x1b[31;1m%s\x1b[0m) \n\x1b[0m", getModel(), getRegion());
	printf("\x1b[31;1m*\x1b[0m Screen type: \x1b[31;1m %s \n\x1b[0m", getScreenType());
	printf("\x1b[31;1m*\x1b[0m Language: \x1b[31;1m%s\x1b[0m \n", getLang());

	printf("\x1b[31;1m*\x1b[0m Device ID: \x1b[31;1m%lu \n", getDeviceId());
	printf("\x1b[31;1m*\x1b[0m ECS Device ID: \x1b[31;1m%llu \n", getSoapId());
	printf("\x1b[31;1m*\x1b[0m Local friend code seed: \x1b[31;1m%010llX\x1b[0m \n", getLocalFriendCodeSeed());	
	printf("\x1b[31;1m*\x1b[0m MAC Address: \x1b[31;1m%s\x1b[0m \n", getMacAddress());
	printf("\x1b[31;1m*\x1b[0m Serial number: \x1b[31;1m%s\x1b[0m \n", getSerialNum());

	FSUSER_GetSdmcCid(buf, 0x10);
	printf("\x1b[31;1m*\x1b[0m SDMC CID: \x1b[31;1m%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\x1b[0m \n",
			buf[0], buf[1], buf[2], buf[3], buf[4], buf[5],
			buf[6], buf[7], buf[8], buf[9], buf[10], buf[11],
			buf[12], buf[13], buf[14], buf[15]);

	FSUSER_GetNandCid(buf, 0x10);
	printf("\x1b[31;1m*\x1b[0m NAND CID: \x1b[31;1m%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\x1b[0m \n",
			buf[0], buf[1], buf[2], buf[3], buf[4], buf[5],
			buf[6], buf[7], buf[8], buf[9], buf[10], buf[11], 
			buf[12], buf[13], buf[14], buf[15]);
			
	ret = APT_GetAppletInfo(APPID_HOMEMENU, &homemenuID, NULL, NULL, NULL, NULL);
	printf("\x1b[31;1m*\x1b[0m Homemenu ID: \x1b[31;1m%016llX\x1b[0m \n\n", homemenuID);
		
	while (aptMainLoop())
	{
		//=====================================================================//
		//----------------------------Battery Info-----------------------------//
		//=====================================================================//
		
		printf("\x1b[19;0H");
		mcuGetBatteryLevel(&batteryPercent);
		printf("\x1b[34;1m*\x1b[0m Battery percentage: \x1b[34;1m%3d%%\x1b[0m (\x1b[34;1m%s\x1b[0m)     \n", batteryPercent, batteryStatus());

		printf("\x1b[20;0H");
		mcuGetBatteryVoltage(&batteryVolt);
		printf("\x1b[34;1m*\x1b[0m Battery voltage: \x1b[34;1m%d\x1b[0m (\x1b[34;1m%.1f V\x1b[0m)    \n", batteryVolt, 5.0 * ((double)batteryVolt / 256.0));//,(Estimated: %0.1lf V) estimatedVolt);
		
		printf("\x1b[21;0H");
		PTMU_GetAdapterState(&isConnected);
		printf("\x1b[34;1m*\x1b[0m Adapter state: \x1b[34;1m%s\x1b[0m\n", isConnected? "connected   " : "disconnected");
		
		printf("\x1b[22;0H");
		GetMcuFwVerHigh(&mcuFwMajor);
		GetMcuFwVerLow(&mcuFwMinor);
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
		printf("\x1b[36;1m*\x1b[0m Installed titles: SD: \x1b[36;1m%lu\x1b[0m  (NAND: \x1b[36;1m%lu\x1b[0m)\n", titleCount(MEDIATYPE_SD), titleCount(MEDIATYPE_NAND));
		
		printf("\x1b[27;0H");
		wifiPercent = (osGetWifiStrength() * 33.3333333333);
		printf("\x1b[36;1m*\x1b[0m WiFi signal strength: \x1b[36;1m%d\x1b[0m  (\x1b[36;1m%.0lf%%\x1b[0m)    \n", osGetWifiStrength(), wifiPercent);

		printf("\x1b[28;0H");
		HIDUSER_GetSoundVolume(&volume);
		volPercent = (volume * 1.5873015873);
		printf("\x1b[36;1m*\x1b[0m Volume slider state: \x1b[36;1m%d\x1b[0m  (\x1b[36;1m%.0lf%%\x1b[0m)    \n", volume, volPercent);

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
	
	free(nver);
	free(cver);
	free(str_ver);
	free(str_sysver);
	
	termServices();
	return 0;
}