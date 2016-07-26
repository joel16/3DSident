#include <3ds.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

const char * getModel()
{
    const char *models[] = {
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
    const char *regions[] = {
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
    const char *languages[] = {
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
    sprintf(macAddress,"%02X:%02X:%02X:%02X:%02X:%02X",*macByte,*(macByte+1),*(macByte+2),*(macByte+3),*(macByte+4),*(macByte+5));

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

    printf("3DSident 0.1\n\n");

    snprintf(str_ver, 255, "* Kernel version: %lu.%lu-%lu\n* FIRM version is %lu.%lu-%lu\n\n",
             GET_VERSION_MAJOR(os_ver), GET_VERSION_MINOR(os_ver), GET_VERSION_REVISION(os_ver),
             GET_VERSION_MAJOR(firm_ver), GET_VERSION_MINOR(firm_ver), GET_VERSION_REVISION(firm_ver));

    printf(str_ver);

    OS_VersionBin *nver = (OS_VersionBin*)malloc(sizeof(OS_VersionBin));
    OS_VersionBin *cver = (OS_VersionBin*)malloc(sizeof(OS_VersionBin));

    memset(nver, 0, sizeof(OS_VersionBin));
    memset(cver, 0, sizeof(OS_VersionBin));

    s32 ret = osGetSystemVersionData(nver, cver);
    if (ret)
        printf("* osGetSystemVersionData returned 0x%08liX\n", ret);

    snprintf(str_sysver, 100, "* Current system version: %d.%d.%d-%d\n",
             cver->mainver, cver->minor, cver->build, nver->mainver);

    printf("* Model: %s\n", getModel());
    printf("* Region: %s\n", getRegion());
    printf("* Language: %s\n", getLang());
    printf("* MAC Address: %s\n", getMacAddress());

    printf("* Battery Status: %s\n", batteryStatus());

    printf("* SD Detected: %s\n", detectSD() ? "Yes" : "No");

    if (!ret)
        printf(str_sysver);

    printf("\n\nPress any key to exit\n");

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
    fsExit();
    cfguExit();
    gfxExit();
    return 0;
}
