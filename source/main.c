#include <3ds.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

char * getModel()
{
	u8 model = 0;
	CFGU_GetSystemModel(&model);
	
	char * modelName;

	switch (model) 
	{
		case 0: 
			modelName = "O3DS"; 
			break;
		case 1: 
			modelName = "O3DS XL"; 
			break;
		case 2: 
			modelName = "N3DS"; 
			break;
		case 3: 
			modelName = "2DS"; 
			break;
		case 4: 
			modelName = "N3DS XL"; 
			break;
		default: 
			modelName = "Unknown";
	}
	
	return modelName;
}

char * getRegion()
{
	u8 region = 0;
	CFGU_SecureInfoGetRegion(&region);
	
	char * regionName;
	
	switch (region) 
	{
		case 0: 
			regionName = "JPN"; 
			break;
		case 1: 
			regionName = "NTSC"; 
			break;
		case 2:
			regionName = "EUR"; 
			break; 
		case 3: 
			regionName = "PAL";
			break; 
		case 4: 
			regionName = "CHN"; 
			break;
		case 5: 
			regionName = "KOR";
			break;
		case 6: 
			regionName = "TWN"; 
			break;
		default: 
			regionName = "Unknown";
	}
	
	return regionName;
}

char * getLang()
{
	u8 language;
	CFGU_GetSystemLanguage(&language);
	
	char * languageName;
	
	switch (language) 
	{
		case 0: 
			languageName = "Japanese"; 
			break;
		case 1: 
			languageName = "English"; 
			break;
		case 2:
			languageName = "French"; 
			break; 
		case 3: 
			languageName = "German";
			break; 
		case 4: 
			languageName = "Italian"; 
			break;
		case 5: 
			languageName = "Simplified Chinese";
			break;
		case 6: 
			languageName = "Korean"; 
			break;
		case 7: 
			languageName = "Dutch"; 
			break;
		case 8: 
			languageName = "Portugese"; 
			break;
		case 9: 
			languageName = "Russian"; 
			break;
		case 10: 
			languageName = "Traditional Chinese"; 
			break;
		default: 
			languageName = "Unknown";
	}
	
	return languageName;
}

bool detectSD()
{
	bool isSD;
	FSUSER_IsSdmcDetected(&isSD);
	return isSD;
}

char * batteryStatus()
{
	u8 batteryStateBool;
	PTMU_GetBatteryChargeState(&batteryStateBool);
	
	char * batteryStat;
	
	if (batteryStateBool == 0) 
		batteryStat = "Not charging";
	else 
		batteryStat = "Charging";
	
	return batteryStat;
}

char * getMacAddress()
{
	u8* macByte = (u8*)0x1FF81060; 
	static char macAddress[18];
	sprintf(macAddress,"%02X:%02X:%02X:%02X:%02X:%02X",*macByte,*(macByte+1),*(macByte+2),*(macByte+3),*(macByte+4),*(macByte+5));
	macAddress[17] = 0;
	
	return macAddress;
}

int main()
{
	gfxInitDefault();
	cfguInit();
	fsInit();
	sdmcInit();
	ptmuInit();
	
    char *str_ver = malloc(100), *str_sysver = malloc(100);
    u32 os_ver = osGetKernelVersion(), firm_ver = osGetKernelVersion();

    consoleInit(GFX_TOP, NULL);
	
	printf("3DSident 0.1\n\n");

    snprintf(str_ver, 100, "* Kernel version: %lu.%lu-%lu\n* FIRM version is %lu.%lu-%lu\n",
             GET_VERSION_MAJOR(os_ver), GET_VERSION_MINOR(os_ver), GET_VERSION_REVISION(os_ver),
             GET_VERSION_MAJOR(firm_ver), GET_VERSION_MINOR(firm_ver), GET_VERSION_REVISION(firm_ver));

    printf(str_ver);

    OS_VersionBin *nver = (OS_VersionBin*)malloc(sizeof(OS_VersionBin));
    OS_VersionBin *cver = (OS_VersionBin*)malloc(sizeof(OS_VersionBin));

    memset(nver, 0, sizeof(OS_VersionBin));
    memset(cver, 0, sizeof(OS_VersionBin));

    s32 ret = osGetSystemVersionData(nver, cver);
    if (ret)
        printf("* osGetSystemVersionData returned 0x%08X\n", (unsigned int) ret);
	
    /*snprintf(str_sysver, 100, "* Current system version: %d.%d.%d-%d\n",
             cver->mainver, cver->minor, cver->build, nver->mainver);*/

	bool sdDetect = detectSD();
	
	
	printf("\n* Model: %s\n", getModel());
	printf("* Region: %s\n", getRegion());
	printf("* Language: %s\n", getLang());
	printf("* MAC Address: %s\n", getMacAddress());
	
	printf("* Battery Status: %s\n", batteryStatus());
	
	if (sdDetect)
		printf("* SD Detected: Yes\n");
	else 
		printf("* MAC Address: No\n");
	
	printf("* MAC Address: No\n");
	
    printf(str_sysver);
    printf("\n\nPress any key to exit");

    free(nver);
    free(cver);
    free(str_ver);
    free(str_sysver);

	while (aptMainLoop())
	{
		gspWaitForVBlank();
		hidScanInput();

		if (hidKeysDown()) break;

		gfxFlushBuffers();
		gfxSwapBuffers();
	}

	ptmuExit();
	sdmcInit();
	fsInit();
	cfguExit();
	gfxExit();
	return 0;
}
