#include <3ds.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <malloc.h>
#include <unistd.h>

#include "actu.h"
#include "am.h"
#include "cfgs.h"
#include "gsplcd.h"
#include "mcu.h"
#include "misc.h"
#include "power.h"
#include "screenshot.h"
#include "system.h"
#include "utils.h"

#define SDK(a,b,c,d) ((a<<24)|(b<<16)|(c<<8)|d)

void initServices()
{
	gfxInitDefault();
	cfguInit();
	cfgsInit();
	fsInit();
	sdmcInit();
	ptmuInit();
	mcuInit();
	amInit();
	amAppInit();
	psInit();
	aptInit();
	hidInit();
	actuInit();
	actInit(SDK(11,2,0,200), 0x20000);
	httpcInit(0x9000);
}

void termServices()
{
	httpcExit();
	actExit();
	actuExit();
	hidExit();
	aptExit();
	psExit();
	acExit();
	amExit();
	mcuExit();
	ptmuExit();
	sdmcExit();
	fsExit();
	cfgsExit();
	cfguExit();
	gfxExit();
}

int main(int argc, char *argv[])
{
	initServices();
	
	consoleInit(GFX_BOTTOM, NULL);
	
	printf("\x1b[36;1m*\x1b[0m Username: \x1b[36;1m %s \n\x1b[0m", getUsername());
	printf("\x1b[36;1m*\x1b[0m Birthday: \x1b[36;1m%s\x1b[0m \n", getBirthday());
	printf("\x1b[36;1m*\x1b[0m EULA version: \x1b[36;1m%s\x1b[0m \n\n ", getEulaVersion());
	
	printf("\n\x1b[32;1m> Press any key to exit =)\x1b[0m");
	//printf("\x1b[31;1m*\x1b[0m Device cert: \x1b[31;1m%s\x1b[0m \n\n", getDeviceCert());
	
	consoleInit(GFX_TOP, NULL);

	//=====================================================================//
	//------------------------Variable Declaration-------------------------//
	//=====================================================================//
	
	char *str_ver = (char *)malloc(sizeof(char) * 255), *str_sysver = (char *)malloc(sizeof(char) * 255);
	double wifiPercent, volPercent, _3dSliderPercent;
	u32 os_ver = osGetKernelVersion(), firm_ver = osGetKernelVersion(), installedTitles = titleCount(MEDIATYPE_SD), nnidNum = 0xFFFFFFFF;
	u8 buf[16], batteryPercent, batteryVolt, volume;
	OS_VersionBin *nver = (OS_VersionBin *)malloc(sizeof(OS_VersionBin)), *cver = (OS_VersionBin *)malloc(sizeof(OS_VersionBin));
	s32 ret;
	FS_ArchiveResource	resource = {0};

	printf("\x1b[0;0H"); //Move the cursor to the top left corner of the screen
	printf("\x1b[32;1m3DSident 0.7.5\x1b[0m\n\n");

	//u32 brightness  = 0;
	//GSPLCD_GetBrightness(brightness);
	
	//printf("\x1b[32;1m*\x1b[0m Brightness: \x1b[32;1m%i\x1b[0m\n", (int)brightness);

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
		printf("\x1b[33;1m*\x1b[0m osGetSystemVersionData returned 0x%08liX\n\n", ret);

	snprintf(str_sysver, 100, "\x1b[33;1m*\x1b[0m Current system version: \x1b[33;1m%d.%d.%d-%d\x1b[0m\n\n",
			cver->mainver,
			cver->minor,
			cver->build,
			nver->mainver
			);

	if (!ret)
		printf(str_sysver);
		
	//=====================================================================//
	//-----------------------------System Info-----------------------------//
	//=====================================================================//
	
	printf("\x1b[31;1m*\x1b[0m Model: \x1b[31;1m%s\x1b[0m (\x1b[31;1m%s\x1b[0m) \n\x1b[0m", getModel(), getRegion());
	printf("\x1b[31;1m*\x1b[0m Screen type: \x1b[31;1m %s \n\x1b[0m", getScreenType());
	printf("\x1b[31;1m*\x1b[0m Language: \x1b[31;1m%s\x1b[0m \n", getLang());
	printf("\x1b[31;1m*\x1b[0m NNID: \x1b[31;1m%s\x1b[0m ", (char*)getNNID());

	ret = ACTU_Initialize(0xB0002C8, 0, 0);
	ret = ACTU_GetAccountDataBlock(0xFE, 4, 12, &nnidNum);

	if (nnidNum != 0xFFFFFFFF)
		printf("(\x1b[31;1m%d\x1b[0m) \n", (int) nnidNum);
	else
		printf("\x1b[31;1mError could not retrieve NNID\x1b[0m\n");

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
	printf("\x1b[31;1m*\x1b[0m NAND CID: \x1b[31;1m%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\x1b[0m \n\n",
			buf[0], buf[1], buf[2], buf[3], buf[4], buf[5],
			buf[6], buf[7], buf[8], buf[9], buf[10], buf[11], 
			buf[12], buf[13], buf[14], buf[15]);
			
	//=====================================================================//
	//----------------------------Battery Info-----------------------------//
	//=====================================================================//
	
	mcuGetBatteryLevel(&batteryPercent);
	printf("\x1b[34;1m*\x1b[0m Battery percentage: \x1b[34;1m%3d%%\x1b[0m (\x1b[34;1m%s\x1b[0m) \n\x1b[0m", batteryPercent, batteryStatus());

	mcuGetBatteryVoltage(&batteryVolt);
	printf("\x1b[34;1m*\x1b[0m Battery voltage: \x1b[34;1m%d\x1b[0m\n", batteryVolt);//,(Estimated: %0.1lf V) estimatedVolt);
	
	u8 mcuFwMajor, mcuFwMinor;
	
	GetMcuFwVerHigh(&mcuFwMajor);
	GetMcuFwVerLow(&mcuFwMinor);
	
	//if (CFG_UNITINFO == 0)
	printf("\x1b[34;1m*\x1b[0m MCU firmware: \x1b[34;1m%u.%u\x1b[0m\n\n", (mcuFwMajor - 16), mcuFwMinor);
		
	//=====================================================================//
	//------------------------------Misc Info------------------------------//
	//=====================================================================//
		
	char sdFreeSize[16], sdTotalSize[16];
	char ctrFreeSize[16], ctrTotalSize[16];	
		
	FSUSER_GetArchiveResource(&resource, SYSTEM_MEDIATYPE_SD);
	getSizeString(sdFreeSize, (((u64) resource.freeClusters * (u64) resource.clusterSize)));
	getSizeString(sdTotalSize, (((u64) resource.totalClusters * (u64) resource.clusterSize)));
	printf("\x1b[32;1m*\x1b[0m SD Size: \x1b[32;1m%s\x1b[0m / \x1b[32;1m%s\x1b[0m \n", sdFreeSize, sdTotalSize);

	FSUSER_GetArchiveResource(&resource, SYSTEM_MEDIATYPE_CTR_NAND);
	getSizeString(ctrFreeSize, (((u64) resource.freeClusters * (u64) resource.clusterSize)));
	getSizeString(ctrTotalSize, (((u64) resource.totalClusters * (u64) resource.clusterSize)));
	printf("\x1b[32;1m*\x1b[0m CTR Size: \x1b[32;1m%s\x1b[0m / \x1b[32;1m%s\x1b[0m \n", ctrFreeSize, ctrTotalSize);

	printf("\x1b[32;1m*\x1b[0m Installed titles: \x1b[32;1m%i\x1b[0m\n", (int)installedTitles);
	
	wifiPercent = (osGetWifiStrength() * 33.3333333333);
	printf("\x1b[32;1m*\x1b[0m WiFi signal strength: \x1b[32;1m%d\x1b[0m  (\x1b[32;1m%.0lf%%\x1b[0m) \n", osGetWifiStrength(), wifiPercent);

	HIDUSER_GetSoundVolume(&volume);
	volPercent = (volume * 1.5873015873);
	printf("\x1b[32;1m*\x1b[0m Volume slider state: \x1b[32;1m%d\x1b[0m  (\x1b[32;1m%.0lf%%\x1b[0m) \n", volume, volPercent);

	_3dSliderPercent = (osGet3DSliderState() * 100.0);
	printf("\x1b[32;1m*\x1b[0m 3D slider state: \x1b[32;1m%.1lf\x1b[0m  (\x1b[32;1m%.0lf%%\x1b[0m)   \n", osGet3DSliderState(), _3dSliderPercent);
	
	printf("\x1b[32;1m*\x1b[0m Brightness: \x1b[32;1m%s\x1b[0m \n", getBrightness(1));

	while (aptMainLoop())
	{
		//=====================================================================//
		//----------------------------Battery Info-----------------------------//
		//=====================================================================//
		
		printf("\x1b[18;0H"); //Move the cursor to the top left corner of the screen
		mcuGetBatteryLevel(&batteryPercent);
		printf("\x1b[34;1m*\x1b[0m Battery percentage: \x1b[34;1m%3d%%\x1b[0m (\x1b[34;1m%s\x1b[0m) \n\x1b[0m", batteryPercent, batteryStatus());

		printf("\x1b[19;0H"); //Move the cursor to the top left corner of the screen
		mcuGetBatteryVoltage(&batteryVolt);
		printf("\x1b[34;1m*\x1b[0m Battery voltage: \x1b[34;1m%d\x1b[0m\n", batteryVolt);//,(Estimated: %0.1lf V) estimatedVolt);
		//=====================================================================//
		//------------------------------Misc Info------------------------------//
		//=====================================================================//
		
		printf("\x1b[24;0H"); // Move the cursor
		wifiPercent = (osGetWifiStrength() * 33.3333333333);
		printf("\x1b[32;1m*\x1b[0m WiFi signal strength: \x1b[32;1m%d\x1b[0m  (\x1b[32;1m%.0lf%%\x1b[0m) \n", osGetWifiStrength(), wifiPercent);

		printf("\x1b[25;0H"); //Move the cursor
		HIDUSER_GetSoundVolume(&volume);
		volPercent = (volume * 1.5873015873);
		printf("\x1b[32;1m*\x1b[0m Volume slider state: \x1b[32;1m%d\x1b[0m  (\x1b[32;1m%.0lf%%\x1b[0m)  \n", volume, volPercent);

		printf("\x1b[26;0H"); //Move the cursor
		_3dSliderPercent = (osGet3DSliderState() * 100.0);
		printf("\x1b[32;1m*\x1b[0m 3D slider state: \x1b[32;1m%.1lf\x1b[0m  (\x1b[32;1m%.0lf%%\x1b[0m)   \n", osGet3DSliderState(), _3dSliderPercent);
		
		gspWaitForVBlank();
		hidScanInput();
		u32 kHeld = hidKeysHeld();
		
		if ((kHeld & KEY_L) && (kHeld & KEY_R))
			captureScreenshot();
		
		else if (hidKeysDown())
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
