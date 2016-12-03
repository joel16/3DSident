#include <3ds.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <malloc.h>
#include <unistd.h>
#include <sf2d.h>
#include <sftd.h>
#include "Verdana_ttf.h"
#include "actu.h"
#include "am.h"
#include "cfgs.h"
#include "frd.h"
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

void getScreenType(sftd_font *font)
{
	bool isNew3DS = 0;
    APT_CheckNew3DS(&isNew3DS);
	
	sftd_draw_text(font, 10, 90,  RGBA8(255, 255 , 0,   255), 10,"Screen info:");
	
    if (isNew3DS)
    {
        u8 screens = 0;
        GSPLCD_GetVendors(&screens);
        switch ((screens >> 4) & 0xF)
        {
            case 1:
				sftd_draw_text(font, 10, 100,  RGBA8(255, 0 , 0,   255), 10,"UPPER :IPS");
                break;
            case 0xC:
                sftd_draw_text(font, 10, 100,  RGBA8(255, 0 , 0,   255), 10,"UPPER :TN");
                break;
            default:
                sftd_draw_text(font, 10, 100,  RGBA8(255, 0 , 0,   255), 10,"UPPER :UNKNOWN");
                break;
        }
        switch (screens & 0xF)
        {
            case 1:
               sftd_draw_text(font, 10, 110,  RGBA8(255, 0 , 0,   255), 10,"LOWER :IPS");
                break;
            case 0xC:
               sftd_draw_text(font, 10, 110,  RGBA8(255, 0 , 0,   255), 10,"LOWER :TN");
                break;
            default:
               sftd_draw_text(font, 10, 110,  RGBA8(255, 0 , 0,   255), 10,"LOWER :UNKNOWN");
                break;
        }
    }
	
    else
    {   sftd_draw_text(font, 10, 100,  RGBA8(255, 0 , 0,   255), 10,"UPPER :TN");
        sftd_draw_text(font, 10, 110,  RGBA8(255, 0 , 0,   255), 10,"LOWER :TN");	
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

int main(int argc, char *argv[])
{   
    sf2d_init();
	sf2d_set_clear_color(RGBA8(0x00, 0x2B, 0x36, 0xFF));
	sf2d_set_vblank_wait(0);
	sftd_init();
	sftd_font *font = sftd_load_font_mem(Verdana_ttf, Verdana_ttf_size);
	//gfxInitDefault();
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
	
	
		
	//printf("\x1b[31;1m*\x1b[0m Device cert: \x1b[31;1m%s\x1b[0m \n\n", getDeviceCert());
	
	//consoleInit(GFX_TOP, NULL);

	//=====================================================================//
	//------------------------Variable Declaration-------------------------//
	//=====================================================================//
	
	char *str_ver = (char *)malloc(sizeof(char) * 255),*str_verf = (char *)malloc(sizeof(char) * 255), *str_sysver = (char *)malloc(sizeof(char) * 255);
	char *sty=(char*)malloc(sizeof(char)* 255);
	double wifiPercent, volPercent, _3dSliderPercent;
	u32 os_ver = osGetKernelVersion(), firm_ver = osGetKernelVersion(), installedTitles = titleCount(MEDIATYPE_SD), nnidNum = 0xFFFFFFFF;
	u8 buf[16], batteryPercent, batteryVolt, volume;
	OS_VersionBin *nver = (OS_VersionBin *)malloc(sizeof(OS_VersionBin)), *cver = (OS_VersionBin *)malloc(sizeof(OS_VersionBin));
	s32 ret;
	FS_ArchiveResource	resource = {0};

	while (aptMainLoop())
	{   
        
		//printf("\x1b[0;0H"); //Move the cursor to the top left corner of the screen
		//printf("\x1b[32;1m3DSident 0.7.1\x1b[0m\n\n");
        sf2d_start_frame(GFX_TOP,GFX_LEFT);
		sftd_draw_text(font, 10, 10,  RGBA8(0, 255, 0,   255), 10,"3DSident 0.7.1");
		
		//=====================================================================//
		//------------------------------Firm Info------------------------------//
		//=====================================================================//
		
		snprintf(str_ver, 255, "Kernel version: %lu.%lu-%lu",
		GET_VERSION_MAJOR(os_ver),
		GET_VERSION_MINOR(os_ver),
		GET_VERSION_REVISION(os_ver)
	    );
		sftd_draw_text(font, 10, 20,  RGBA8(255 ,140 ,0 ,255), 10,str_ver);
		
		snprintf(str_verf,255, "FIRM version is: %lu.%lu-%lu",
				GET_VERSION_MAJOR(firm_ver),
				GET_VERSION_MINOR(firm_ver),
				GET_VERSION_REVISION(firm_ver)
				);
        sftd_draw_text(font, 10, 30,  RGBA8(0,255 ,255,   255), 10,str_verf);
		

		memset(nver, 0, sizeof(OS_VersionBin));
		memset(cver, 0, sizeof(OS_VersionBin));
		ret = osGetSystemVersionData(nver, cver);
		if(ret){
		//printf("osGetSystemVersionData returned 0x%08liX\n\n", ret);
		sprintf(sty,"osGetSystemVersionData returned 0x%08liX",ret);
        sftd_draw_text(font, 10, 40,  RGBA8(0, 255, 0,   255), 10,sty);
	          }
		snprintf(str_sysver, 100, "Current system version:%d.%d.%d-%d",
				cver->mainver,
				cver->minor,
				cver->build,
				nver->mainver
				);
        if (!ret)
		sftd_draw_text(font, 10, 40,  RGBA8(0, 255, 0,   255), 10,str_sysver);
			
		
		//=====================================================================//
		//-----------------------------System Info-----------------------------//
		//=====================================================================//
		sftd_draw_textf(font, 10, 50,  RGBA8(0, 255, 255,   255), 10,"Model: %s",getModel());
		//printf("\x1b[31;1m*\x1b[0m Model: \x1b[31;1m%s %s\n\x1b[0m", getModel(), getRegion());
		sftd_draw_textf(font, 10, 60,  RGBA8(255 ,140 ,0 ,255), 10,"Region: %s",getRegion());
		
		sftd_draw_textf(font, 10, 70,  RGBA8(255, 0 , 0,   255), 10,"%s",getLang());
		//printf("\x1b[31;1m*\x1b[0m Language: \x1b[31;1m%s\x1b[0m \n", getLang());
		sftd_draw_textf(font, 10, 80,  RGBA8(255, 0 , 0,   255), 10,"NNID: %s",(char*)getNNID());
		//printf("\x1b[31;1m*\x1b[0m NNID: \x1b[31;1m%s\x1b[0m ", (char*)getNNID());
        getScreenType(font);
		ret = ACTU_Initialize(0xB0002C8, 0, 0);
		ret = ACTU_GetAccountDataBlock(0xFE, 4, 12, &nnidNum);
   
		//if (nnidNum != 0xFFFFFFFF)
		//	vaPrint("(\x1b[31;1m%08X\x1b[0m) \n", (int) nnidNum);
	        // sftd_draw_textf(font, 40, 80,  RGBA8(0, 255, 0,   255), 10,"%08X",(int)nnidNum);
	   // else
			//sftd_draw_text(font, 20, 80,  RGBA8(0, 255, 0,   255), 10,"Error could not retrieve NNID");
			//printf("\x1b[31;1mError could not retrieve NNID\x1b[0m\n");
        sftd_draw_textf(font, 10, 120,  RGBA8(120, 255 , 0,   255), 10,"Device ID: %lu",getDeviceId);
		//printf("\x1b[31;1m*\x1b[0m Device ID: \x1b[31;1m%lu \n", getDeviceId());
		sftd_draw_textf(font, 10, 130,  RGBA8(255, 0 , 0,   255), 10,"ECS device ID: %llu",getSoapId());
		//printf("\x1b[31;1m*\x1b[0m ECS Device ID: \x1b[31;1m%llu \n", getSoapId());
		sftd_draw_textf(font, 10, 140,  RGBA8(255, 0 , 0,   255), 10,"Friend Code: %llu",principalIdToFriendCode(getMyFriendKey().principalId));
		//
		sftd_draw_textf(font, 10, 150,  RGBA8(120, 120 , 90,   255), 10,"MAC Address: %s",getMacAddress());
		sftd_draw_textf(font, 10, 160,  RGBA8(255, 0 , 0,   255), 10,"Serial Number %s",getSerialNum());
		//printf("\x1b[31;1m*\x1b[0m Friend Code: \x1b[31;1m%llu \n", principalIdToFriendCode(getMyFriendKey().principalId));
		//printf("\x1b[31;1m*\x1b[0m MAC Address: \x1b[31;1m%s\x1b[0m \n", getMacAddress());
		//printf("\x1b[31;1m*\x1b[0m Serial number: \x1b[31;1m%s\x1b[0m \n", getSerialNum());

		FSUSER_GetSdmcCid(buf, 0x10);
		sftd_draw_textf(font, 10, 170,  RGBA8(255, 70 , 90,   255), 10,"SDMC CID %02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",buf[0], buf[1], buf[2], buf[3], buf[4], buf[5],
				buf[6], buf[7], buf[8], buf[9], buf[10], buf[11],
				buf[12], buf[13], buf[14], buf[15]);
		/*printf("\x1b[31;1m*\x1b[0m SDMC CID: \x1b[31;1m%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\x1b[0m \n",
				buf[0], buf[1], buf[2], buf[3], buf[4], buf[5],
				buf[6], buf[7], buf[8], buf[9], buf[10], buf[11],
				buf[12], buf[13], buf[14], buf[15]);
        */
		
		FSUSER_GetNandCid(buf, 0x10);
		sftd_draw_textf(font, 10, 180,  RGBA8(25, 70 , 90,   255), 10,"NAND CID %02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",buf[0], buf[1], buf[2], buf[3], buf[4], buf[5],
				buf[6], buf[7], buf[8], buf[9], buf[10], buf[11],
				buf[12], buf[13], buf[14], buf[15]);
		//=====================================================================//
		//----------------------------Battery Info-----------------------------//
		//=====================================================================//
		
		mcuGetBatteryLevel(&batteryPercent);
		sftd_draw_textf(font, 10, 190,  RGBA8(255 ,140 ,0 ,255), 10,"Battery Percentage: %3d%% Battery Status %s", batteryPercent,batteryStatus());
		//printf("\x1b[34;1m*\x1b[0m Battery percentage: \x1b[34;1m%3d%%\x1b[0m (\x1b[34;1m%s\x1b[0m) \n\x1b[0m", batteryPercent, batteryStatus());
		

		mcuGetBatteryVoltage(&batteryVolt);
		sftd_draw_textf(font, 10, 200,  RGBA8(255 ,140 ,0 ,255), 10,"Battery Voltage %d",batteryVolt);
		//printf("\x1b[34;1m*\x1b[0m Battery voltage: \x1b[34;1m%d\x1b[0m\n \n", batteryVolt);//,(Estimated: %0.1lf V) estimatedVolt);
		
		//=====================================================================//
		//------------------------------Misc Info------------------------------//
		//=====================================================================//
		
		FSUSER_GetArchiveResource(&resource, SYSTEM_MEDIATYPE_SD);
		sftd_draw_textf(font, 10, 210,  RGBA8(120, 234 , 255,   255), 10,"SD Size: %.1f MB/%.1f MB",
				(((u64) resource.freeClusters * (u64) resource.clusterSize) / 1024.0 / 1024.0),
				(((u64) resource.totalClusters * (u64) resource.clusterSize) / 1024.0 / 1024.0));

		FSUSER_GetArchiveResource(&resource, SYSTEM_MEDIATYPE_CTR_NAND);
		sftd_draw_textf(font, 10, 220,  RGBA8(120, 234 , 255,   255), 10,"CTR NAND Size: %.1f MB/%.1f MB", 
				(((u64) resource.freeClusters * (u64) resource.clusterSize) / 1024.0 / 1024.0),
				(((u64) resource.totalClusters * (u64) resource.clusterSize) / 1024.0 / 1024.0));

	   sftd_draw_textf(font, 10,  230,  RGBA8(120, 234 , 255,   255), 10,"No. of titles: %i", (int)installedTitles);
       sf2d_end_frame();
	   
	   //--bottom screen--
	   sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
        sftd_draw_textf(font, 10, 10,  RGBA8(255,   0, 0,   255), 10, "%s",getDeviceCert());
	   
		wifiPercent = (osGetWifiStrength() * 33.3333333333);
		sftd_draw_textf(font, 10, 20,  RGBA8(120, 255 , 15,   255), 10,"WiFi signal strength: %d (%.0lf%%)",osGetWifiStrength(),wifiPercent);
		//printf("\x1b[32;1m*\x1b[0m WiFi signal strength: \x1b[32;1m%d\x1b[0m  (\x1b[32;1m%.0lf%%\x1b[0m) \n", osGetWifiStrength(), wifiPercent);

		mcuGetVolume(&volume);
		volPercent = (volume * 1.5873015873);
		sftd_draw_textf(font, 10, 30,  RGBA8(120, 255 , 15,   255), 10,"Volume slider state: %d (%.0lf%%)",volume, volPercent);
		//printf("\x1b[32;1m*\x1b[0m Volume slider state: \x1b[32;1m%d\x1b[0m  (\x1b[32;1m%.0lf%%\x1b[0m) \n", volume, volPercent);

		_3dSliderPercent = (osGet3DSliderState() * 100.0);
		sftd_draw_textf(font, 10, 40,  RGBA8(120, 255 , 15,   255), 10,"3D slider state: %.1lf(%.0lf%%)",osGet3DSliderState(), _3dSliderPercent);
		//printf("\x1b[32;1m*\x1b[0m 3D slider state: \x1b[32;1m%.1lf\x1b[0m  (\x1b[32;1m%.0lf%%\x1b[0m)   \n", osGet3DSliderState(), _3dSliderPercent);
		sftd_draw_text(font, 10, 50,  RGBA8(120, 255 , 15,   255), 10,"> Press any key to exit =)");
		//printf("\n\x1b[32;1m> Press any key to exit =)\x1b[0m");
        
		//gspWaitForVBlank();
		hidScanInput();
		
		if (hidKeysDown())
		{
			captureScreenshot();
			break;
		}
		
		//gfxFlushBuffers();
		//gfxSwapBuffers();
		sf2d_end_frame();
       sf2d_swapbuffers();
	}
	
	free(nver);
	free(cver);
	free(str_ver);
	free(sty);
	free(str_sysver);
	
    frdExit();
	httpcExit();
	gspLcdExit();
	actuExit();
	acExit();
	hidExit();
	aptExit();
	psExit();
	amExit();
	mcuExit();
	ptmuExit();
	sdmcExit();
	fsExit();
	cfgsExit();
	cfguExit();
	sftd_free_font(font);
	sftd_fini();
	sf2d_fini();
	//gfxExit();
	return 0;
}
