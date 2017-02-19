#include <3ds.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <malloc.h>
#include <unistd.h>

#include "actu.h"
#include "am.h"
#include "cfgs.h"
#include "frd.h"
#include "gsplcd.h"
#include "mcu.h"
#include "screenshot.h"
#include "utils.h"

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

    //sprintf automatically zero-terminates the string
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
	
	printf("\x1b[31;1m*\x1b[0m Screen Info: ");
	
    if (isNew3DS)
    {
        u8 screens = 0;
        GSPLCD_GetVendors(&screens);
        switch ((screens >> 4) & 0xF)
        {
            case 1:
				printf("Upper: \x1b[31;1mIPS\x1b[0m ");
                break;
            case 0xC:
                printf("Upper: \x1b[31;1mTN\x1b[0m ");
                break;
            default:
                printf("Upper: \x1b[31;1mUnknown \x1b[0m");
                break;
        }
        switch (screens & 0xF)
        {
            case 1:
                printf("| Lower: \x1b[31;1mIPS\x1b[0m\n");
                break;
            case 0xC:
                printf("| Lower: \x1b[31;1mTN\x1b[0m\n");
                break;
            default:
                printf("| Lower: \x1b[31;1mUnknown\x1b[0m\n");
                break;
        }
    }
	
    else
    {
        printf("Upper: \x1b[31;1mTN\x1b[0m | Lower: \x1b[31;1mTN\n");
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

char * getDeviceCert(void)
{
    u8 const cert[0x180];
    amNetGetDeviceCert(cert);
    return base64Encode(cert);
}

char * getNNID(void)
{
    static char tmp[0x11];
    ACT_GetAccountInfo(tmp, 0x11, 0x8);
	
    return tmp;
}

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
	gspLcdInit();
	httpcInit(0x9000);
	frdInit(SDK(11,4,0,200));
}

void termServices()
{
	frdExit();
	httpcExit();
	gspLcdExit();
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
	
	/*consoleInit(GFX_BOTTOM, NULL);
		
	printf("\x1b[31;1m*\x1b[0m Device cert: \x1b[31;1m%s\x1b[0m \n\n", getDeviceCert());*/
	
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
	printf("\x1b[32;1m3DSident 0.7.2\x1b[0m\n\n");

	
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
	
	printf("\x1b[31;1m*\x1b[0m Model: \x1b[31;1m%s %s\n\x1b[0m", getModel(), getRegion());
	getScreenType();
	printf("\x1b[31;1m*\x1b[0m Language: \x1b[31;1m%s\x1b[0m \n", getLang());
	printf("\x1b[31;1m*\x1b[0m NNID: \x1b[31;1m%s\x1b[0m ", (char*)getNNID());

	ret = ACTU_Initialize(0xB0002C8, 0, 0);
	ret = ACTU_GetAccountDataBlock(0xFE, 4, 12, &nnidNum);

	if (nnidNum != 0xFFFFFFFF)
		vaPrint("(\x1b[31;1m%08X\x1b[0m) \n", (int) nnidNum);
	else
		printf("\x1b[31;1mError could not retrieve NNID\x1b[0m\n");

	printf("\x1b[31;1m*\x1b[0m Device ID: \x1b[31;1m%lu \n", getDeviceId());
	printf("\x1b[31;1m*\x1b[0m ECS Device ID: \x1b[31;1m%llu \n", getSoapId());
	printf("\x1b[31;1m*\x1b[0m Friend Code: \x1b[31;1m%llu \n", principalIdToFriendCode(getMyFriendKey().principalId));
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
		
	FSUSER_GetArchiveResource(&resource, SYSTEM_MEDIATYPE_SD);
	printf("\x1b[32;1m*\x1b[0m SD Size: \x1b[32;1m%.1f \x1b[0mMB / \x1b[32;1m%.1f\x1b[0m MB \n",
			(((u64) resource.freeClusters * (u64) resource.clusterSize) / 1024.0 / 1024.0),
			(((u64) resource.totalClusters * (u64) resource.clusterSize) / 1024.0 / 1024.0));

	FSUSER_GetArchiveResource(&resource, SYSTEM_MEDIATYPE_CTR_NAND);
	printf("\x1b[32;1m*\x1b[0m CTR Size: \x1b[32;1m%.1f\x1b[0m MB / \x1b[32;1m%.1f\x1b[0m MB \n",
			(((u64) resource.freeClusters * (u64) resource.clusterSize) / 1024.0 / 1024.0),
			(((u64) resource.totalClusters * (u64) resource.clusterSize) / 1024.0 / 1024.0));

	printf("\x1b[32;1m*\x1b[0m Installed titles: \x1b[32;1m%i\x1b[0m\n", (int)installedTitles);
	
	wifiPercent = (osGetWifiStrength() * 33.3333333333);
	printf("\x1b[32;1m*\x1b[0m WiFi signal strength: \x1b[32;1m%d\x1b[0m  (\x1b[32;1m%.0lf%%\x1b[0m) \n", osGetWifiStrength(), wifiPercent);

	mcuGetVolume(&volume);
	volPercent = (volume * 1.5873015873);
	printf("\x1b[32;1m*\x1b[0m Volume slider state: \x1b[32;1m%d\x1b[0m  (\x1b[32;1m%.0lf%%\x1b[0m) \n", volume, volPercent);

	_3dSliderPercent = (osGet3DSliderState() * 100.0);
	printf("\x1b[32;1m*\x1b[0m 3D slider state: \x1b[32;1m%.1lf\x1b[0m  (\x1b[32;1m%.0lf%%\x1b[0m)   \n", osGet3DSliderState(), _3dSliderPercent);
		
	printf("\n\x1b[32;1m> Press any key to exit =)\x1b[0m");

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
		mcuGetVolume(&volume);
		volPercent = (volume * 1.5873015873);
		printf("\x1b[32;1m*\x1b[0m Volume slider state: \x1b[32;1m%d\x1b[0m  (\x1b[32;1m%.0lf%%\x1b[0m)  \n", volume, volPercent);

		printf("\x1b[26;0H"); //Move the cursor
		_3dSliderPercent = (osGet3DSliderState() * 100.0);
		printf("\x1b[32;1m*\x1b[0m 3D slider state: \x1b[32;1m%.1lf\x1b[0m  (\x1b[32;1m%.0lf%%\x1b[0m)   \n", osGet3DSliderState(), _3dSliderPercent);
		
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
	
	free(nver);
	free(cver);
	free(str_ver);
	free(str_sysver);
	
	termServices();
	return 0;
}
