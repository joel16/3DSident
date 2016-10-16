#include <3ds.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "actu.h"
#include "cfgs.h"
#include "frd.h"
#include "mcu.h"
#include "screenshot.h"

#define SDK(a,b,c,d) ((a<<24)|(b<<16)|(c<<8)|d)

int vaPrint(char *format, ...)
{
    va_list args;
    va_start(args, format);
	int ret = vprintf(format, args);
    va_end(args);
	gfxFlushBuffers();
	gfxSwapBuffers();
	return ret;
}

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

u32 titleCount(FS_MediaType mediaType)
{
	u32 count = 0;
	
	AM_GetTitleCount(mediaType, &count);

    return count;
}

void getScreenType()
{
	bool isNew3DS = 0;
    APT_CheckNew3DS(&isNew3DS);
	
	printf("\x1b[31m*\x1b[0m Screen Info: ");
	
    if (isNew3DS)
    {
        u8 screens = 0;
        GSPLCD_GetVendors(&screens);
        switch ((screens >> 4) & 0xF)
        {
            case 1:
                printf("Upper: IPS ");
                break;
            case 0xC:
                printf("Upper: TN ");
                break;
            default:
                printf("Upper: Unknown ");
                break;
        }
         switch (screens & 0xF)
        {
            case 1:
                printf("| Lower: IPS\n");
                break;
            case 0xC:
                printf("| Lower: TN\n");
                break;
            default:
                printf("| Lower: Unknown\n");
                break;
        }
    }
	
    else
    {
        printf("Upper: TN | Lower: TN\n");
    }
}

u64 principalIdToFriendCode(u64 pid)
{
    u64 fc = 0;
	
    frdPrincipalIdToFriendCode(&fc, pid);
    
	return fc;
}

FriendKey getMyFriendKey(void)
{
    FriendKey fk;
    
	frdGetMyFriendKey(&fk);
    
	return fk;
}

char * getSerialNum(void)
{
	static char str[32];
    char serial[0x10];
	
    cfgsSecureInfoGetSerialNo(serial);
    strcpy(str, serial);
    
	return str;
}

u32 getDeviceId(void)
{
    u32 tmp = 0;
    AM_GetDeviceId(&tmp);
    return tmp;
}

u64 getSoapId(void)
{
    u32 tmp = 0;
    AM_GetDeviceId(&tmp);
    return (tmp | (((u64) 4) << 32));
}

int main(int argc, char *argv[])
{      
    gfxInitDefault();
    cfguInit();
	cfgsInit();
	//frdInit(SDK(11,4,0,200));
    fsInit();
    sdmcInit();
    ptmuInit();
	mcuInit();
	amInit();
	psInit();
	aptInit();
    hidInit();
	acInit();
    gspLcdInit();
	
    consoleInit(GFX_TOP, NULL);

    char *str_ver = malloc(255), *str_sysver = malloc(255);
    u32 os_ver = osGetKernelVersion(), firm_ver = osGetKernelVersion();

    printf("\x1b[32m3DSident 0.7\x1b[0m\n\n");

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

    printf("\x1b[31m*\x1b[0m Model: %s %s\n", getModel(), getRegion());
	getScreenType();
	//printf("\x1b[31m*\x1b[0m Friend key: %llu\n", principalIdToFriendCode(getMyFriendKey().principalId));
	printf("\x1b[31m*\x1b[0m Language: %s\n", getLang());
	
	u32 nnidNum = 0xFFFFFFFF;
    ret = actInit();
	/*if (ret) 
		vaPrint("actInit failed! %08x\n", ret);*/
    ret = ACTU_Initialize(0xB0002C8, 0, 0);
	/*if (ret) 
		vaPrint("ACTU_Initialize failed! %08x\n", ret);*/
    ret = ACTU_GetAccountDataBlock(0xFE, 4, 12, &nnidNum);
	/*if (ret) 
		vaPrint("ACTU_GetAccountDataBlock failed! %08x\n", ret);*/
    ret = actExit();
	/*if (ret) 
		vaPrint("actExit failed! %08x\n", ret);*/

	if (nnidNum != 0xFFFFFFFF) 
	{
		vaPrint("\x1b[31m*\x1b[0m NNID number: %08X\n", (int) nnidNum);
	}
	else 
	{
		vaPrint("\x1b[31m*\x1b[0m NNID number: Error could not retrieve NNID\n");
	}
    printf("\x1b[31m*\x1b[0m Device ID: (%lu)\n", getDeviceId());
	printf("\x1b[31m*\x1b[0m ECS Device ID: (%llu)\n", getSoapId());
	printf("\x1b[31m*\x1b[0m MAC Address: %s\n", getMacAddress());
	printf("\x1b[31m*\x1b[0m Serial number: %s\n", getSerialNum());
	
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
		
	/*u32 deviceId;
	PS_GetDeviceId(&deviceId);
	printf("\x1b[31m*\x1b[0m Device ID: %lu\n", deviceId);*/
	
	u8 batteryPercent; 
	mcuGetBatteryLevel(&batteryPercent);
	printf("\x1b[34m*\x1b[0m Battery percentage: %d%% (%s)\n", batteryPercent, batteryStatus());
	
	u8 batteryVolt; 
	mcuGetBatteryVoltage(&batteryVolt);
	//double estimatedVolt = (batteryVolt * 0.02248803827);
	printf("\x1b[34m*\x1b[0m Battery voltage: %d\n\n", batteryVolt);//,(Estimated: %0.1lf V) estimatedVolt);
	
	//printf("\x1b[32m*\x1b[0m SD Detected: %s\n", detectSD() ? "Yes" : "No"); Don't need this
	
	FS_ArchiveResource resource = {0};
	
	FSUSER_GetArchiveResource(&resource, SYSTEM_MEDIATYPE_SD);
	printf("\x1b[32m*\x1b[0m SD Size: %.1f MB / %.1f MB\n", (((u64) resource.freeClusters * (u64) resource.clusterSize) / 1024.0 / 1024.0), (((u64) resource.totalClusters * (u64) resource.clusterSize) / 1024.0 / 1024.0));
	
	FSUSER_GetArchiveResource(&resource, SYSTEM_MEDIATYPE_CTR_NAND);
	printf("\x1b[32m*\x1b[0m CTR Size: %.1f MB / %.1f MB\n", (((u64) resource.freeClusters * (u64) resource.clusterSize) / 1024.0 / 1024.0), (((u64) resource.totalClusters * (u64) resource.clusterSize) / 1024.0 / 1024.0));
	
	u32 installedTitles = titleCount(MEDIATYPE_SD);
	printf("\x1b[32m*\x1b[0m Installed titles: %i\n", (int)installedTitles);
	
	/*u32 wifiStatus = 0;
	ACU_GetWifiStatus(&wifiStatus);
	int wifiStat = wifiStatus + osGetWifiStrength();*/
	double wifiPercent = (osGetWifiStrength() * 33.3333333333);
	printf("\x1b[32m*\x1b[0m WiFi signal strength: %d  (%.0lf%%)\n", osGetWifiStrength(), wifiPercent);
	
	u8 volume; 
	mcuGetVolume(&volume);
	double volPercent = (volume * 1.5873015873);
	printf("\x1b[32m*\x1b[0m Volume slider state: %d  (%.0lf%%)\n", volume, volPercent);
	
	double _3dSliderPercent = (osGet3DSliderState() * 100.0);
	printf("\x1b[32m*\x1b[0m 3D slider state: %.1lf  (%.0lf%%)\n", osGet3DSliderState(), _3dSliderPercent);

	printf("\n\x1b[32m> Press any key to exit =)\x1b[0m\n");
	
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

	gspLcdExit();
	acExit();
	hidExit();
	aptExit();
	psExit();
	amExit();
	mcuExit();
    ptmuExit();
    sdmcExit();
    fsExit();
    //frdExit();
	cfgsExit();
    cfguExit();
    gfxExit();
    return 0;
}
