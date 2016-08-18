#include <3ds.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "actu.h"
#include "screenshot.h"

const char * getModel()
{
    const char *models[] = 
	{
        "O3DS",
        "O3DS XL",
        "N3DS",
        "2DS",
        "N3DS XL",
        "Unknown"
    };

    u8 model = 0;
    CFGU_GetSystemModel(&model);

    if (model < 5)
        return models[model];
    else
        return models[5];
}

const char * getRegion()
{
    const char *regions[] = 
	{
        "JPN",
        "USA",
        "EUR",
        "AUS",
        "CHN",
        "KOR",
        "TWN",
        "Unknown"
    };

    u8 region = 0;
    CFGU_SecureInfoGetRegion(&region);

    if (region < 7)
        return regions[region];
    else
        return regions[7];
}

const char * getLang()
{
    const char *languages[] = 
	{
        "Japanese",
        "English",
        "French",
        "German",
        "Italian",
        "Spanish",
        "Simplified Chinese",
        "Korean",
        "Dutch",
        "Portugese",
        "Russian",
        "Traditional Chinese"
    };

    u8 language;
    CFGU_GetSystemLanguage(&language);

    if (language < 11)
        return languages[language];
    else
        return languages[11];
}

bool detectSD()
{
    bool isSD;
    FSUSER_IsSdmcDetected(&isSD);
    return isSD;
}

const char * batteryStatus()
{
    u8 batteryStateBool;
    PTMU_GetBatteryChargeState(&batteryStateBool);

    if (!batteryStateBool) 
        return "Not charging";
    else 
        return "Charging";
}

char * getMacAddress()
{
    u8* macByte = (u8*)0x1FF81060; 
    static char macAddress[18];

    // sprintf automatically zero-terminates the string
    sprintf(macAddress, "%02X:%02X:%02X:%02X:%02X:%02X", *macByte, *(macByte + 1), *(macByte + 2), *(macByte + 3), *(macByte + 4), *(macByte + 5));

    return macAddress;
}

int main(int argc, char *argv[])
{
    gfxInitDefault();
    cfguInit();
    fsInit();
    sdmcInit();
    ptmuInit();

    consoleInit(GFX_TOP, NULL);

    char *str_ver = malloc(255), *str_sysver = malloc(255);
    u32 os_ver = osGetKernelVersion(), firm_ver = osGetKernelVersion();

    printf("\x1b[32m3DSident 0.3.1\x1b[0m\n\n");

    snprintf(str_ver, 255, "\x1b[33m*\x1b[0m Kernel version: %lu.%lu-%lu\n\x1b[33m*\x1b[0m FIRM version is %lu.%lu-%lu\n",
             GET_VERSION_MAJOR(os_ver), GET_VERSION_MINOR(os_ver), GET_VERSION_REVISION(os_ver),
             GET_VERSION_MAJOR(firm_ver), GET_VERSION_MINOR(firm_ver), GET_VERSION_REVISION(firm_ver));

    printf(str_ver);

    OS_VersionBin *nver = (OS_VersionBin*)malloc(sizeof(OS_VersionBin));
    OS_VersionBin *cver = (OS_VersionBin*)malloc(sizeof(OS_VersionBin));

    memset(nver, 0, sizeof(OS_VersionBin));
    memset(cver, 0, sizeof(OS_VersionBin));

    s32 ret = osGetSystemVersionData(nver, cver);
    if (ret)
        printf("\x1b[33m*\x1b[0m osGetSystemVersionData returned 0x%08liX\n\n", ret);

    snprintf(str_sysver, 100, "\x1b[33m*\x1b[0m Current system version: %d.%d.%d-%d\n\n",
             cver->mainver, cver->minor, cver->build, nver->mainver);
	if (!ret)
        printf(str_sysver);

    printf("\x1b[31m*\x1b[0m Model: %s\n", getModel());
    printf("\x1b[31m*\x1b[0m Region: %s\n", getRegion());
    printf("\x1b[31m*\x1b[0m Language: %s\n", getLang());
    printf("\x1b[31m*\x1b[0m MAC Address: %s\n", getMacAddress());
	
	u8 buf[16];
	
	FSUSER_GetSdmcCid(buf, 0x10);
	printf("\x1b[31m*\x1b[0m SDMC CID: %02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\n",
		buf[0], buf[1], buf[2], buf[3], buf[4], buf[5],
		buf[6], buf[7], buf[8], buf[9], buf[10], buf[11], 
		buf[12], buf[13], buf[14], buf[15]);
	
	FSUSER_GetNandCid(buf, 0x10);
	printf("\x1b[31m*\x1b[0m NAND CID: %02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\n\n",
		buf[0], buf[1], buf[2], buf[3], buf[4], buf[5],
		buf[6], buf[7], buf[8], buf[9], buf[10], buf[11], 
		buf[12], buf[13], buf[14], buf[15]);

	u8 batteryPercent;
	PTMU_GetBatteryLevel(&batteryPercent);
	int batt = (u32)batteryPercent * 20;
    printf("\x1b[34m*\x1b[0m Battery Status: %s\n", batteryStatus());
	printf("\x1b[34m*\x1b[0m Battery Percentage: %d%%\n\n", batt);
	
	actInit();
	ACTU_Initialize(0xB0002C8, 0, 0);
	u32 nnidNum = 0xFFFFFFFF;
	ACTU_GetAccountDataBlock(0xFE, 4, 12, &nnidNum);
	actExit();
	
	if (nnidNum != 0xFFFFFFFF)
		printf("\x1b[34m*\x1b[0m NNID: %08X\n\n", (int) nnidNum); 
	else
		printf("\x1b[34m*\x1b[0m NNID: Error could not retrieve NNID\n\n");
	
	printf("\x1b[32m*\x1b[0m SD Detected: %s\n", detectSD() ? "Yes" : "No");
	
	FS_ArchiveResource resource = {0};
	FSUSER_GetArchiveResource(&resource, SYSTEM_MEDIATYPE_SD);
	
	printf("\x1b[32m*\x1b[0m SD Size: %.1f MB\n", (((u64) resource.totalClusters * (u64) resource.clusterSize) / 1024.0 / 1024.0));
	printf("\x1b[32m*\x1b[0m SD Free: %.1f MB\n", ((u64) resource.freeClusters * (u64) resource.clusterSize) / 1024.0 / 1024.0);
	
	FSUSER_GetArchiveResource(&resource, SYSTEM_MEDIATYPE_CTR_NAND);
	printf("\x1b[32m*\x1b[0m CTR Size: %.1f MB\n", (((u64) resource.totalClusters * (u64) resource.clusterSize) / 1024.0 / 1024.0));
	printf("\x1b[32m*\x1b[0m CTR Free: %.1f MB\n", ((u64) resource.freeClusters * (u64) resource.clusterSize) / 1024.0 / 1024.0);

	printf("\n\n\x1b[32m> Press any key to exit =)\x1b[0m\n");
	
    free(nver);
    free(cver);
    free(str_ver);
    free(str_sysver);

    while (aptMainLoop())
    {
        gspWaitForVBlank();
        hidScanInput();
        if (hidKeysDown())
		{
			captureScreenshot();
			break;
        }
		gfxFlushBuffers();
        gfxSwapBuffers();
    }

    ptmuExit();
    sdmcInit();
    fsExit();
    cfguExit();
    gfxExit();
    return 0;
}
