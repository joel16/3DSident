#include "actu.h"
#include "am.h"
#include "cfgs.h"
#include "frd.h"
#include "main.h"
#include "mcu.h"
#include "misc.h"
#include "power.h"
#include "screenshot.h"
#include "system.h"
#include "utils.h"

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

void kernelMenu()
{
	char *str_kernel = (char *)malloc(sizeof(char) * 255), *str_ver = (char *)malloc(sizeof(char) * 255), *str_sysver = (char *)malloc(sizeof(char) * 255);
	u32 os_ver = osGetKernelVersion(), firm_ver = osGetKernelVersion();
	OS_VersionBin *nver = (OS_VersionBin *)malloc(sizeof(OS_VersionBin)), *cver = (OS_VersionBin *)malloc(sizeof(OS_VersionBin));
	s32 ret;
	u8 buf[16];
	
	sftd_draw_textf(font, 165, 100, RGBA8(0, 0, 0, 255), 12, "Kernel Menu");
	
	snprintf(str_kernel, 255, "Kernel version: %lu.%lu-%lu",
			GET_VERSION_MAJOR(os_ver),
			GET_VERSION_MINOR(os_ver),
			GET_VERSION_REVISION(os_ver)
	);
	
	snprintf(str_ver, 255, "FIRM version is: %lu.%lu-%lu\n",
			GET_VERSION_MAJOR(firm_ver),
			GET_VERSION_MINOR(firm_ver),
			GET_VERSION_REVISION(firm_ver)
	);
	
	sftd_draw_textf(font, 20, 116, RGBA8(77, 76, 74, 255), 12, "%s", str_kernel);
	sftd_draw_textf(font, 20, 132, RGBA8(77, 76, 74, 255), 12, "%s", str_ver);
				
	memset(nver, 0, sizeof(OS_VersionBin));
	memset(cver, 0, sizeof(OS_VersionBin));
	ret = osGetSystemVersionData(nver, cver);

	if (ret)
		sftd_draw_textf(font, 20, 148, RGBA8(77, 76, 74, 255), 12, "osGetSystemVersionData returned 0x%08liX", ret);
	
	snprintf(str_sysver, 100, "Current system version: %d.%d.%d-%d",
			cver->mainver,
			cver->minor,
			cver->build,
			nver->mainver
	);
	
	if (!ret)
		sftd_draw_textf(font, 20, 148, RGBA8(77, 76, 74, 255), 12, "%s", str_sysver);
	
	FSUSER_GetSdmcCid(buf, 0x10);
	sftd_draw_textf(font, 20, 164, RGBA8(77, 76, 74, 255), 12, "SDMC CID: %02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
			buf[0], buf[1], buf[2], buf[3], buf[4], buf[5],
			buf[6], buf[7], buf[8], buf[9], buf[10], buf[11],
			buf[12], buf[13], buf[14], buf[15]);

	FSUSER_GetNandCid(buf, 0x10);
	sftd_draw_textf(font, 20, 180, RGBA8(77, 76, 74, 255), 12, "NAND CID: %02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
			buf[0], buf[1], buf[2], buf[3], buf[4], buf[5],
			buf[6], buf[7], buf[8], buf[9], buf[10], buf[11], 
			buf[12], buf[13], buf[14], buf[15]);
			
	sftd_draw_textf(font, 20, 196, RGBA8(77, 76, 74, 255), 12, "NNID: %s", (char*)getNNID());
	sftd_draw_textf(font, 20, 212, RGBA8(77, 76, 74, 255), 12, "Device ID: %lu", getDeviceId());
}

void systemMenu()
{
	sftd_draw_textf(font, 165, 100, RGBA8(0, 0, 0, 255), 12, "System Menu");
	
	sftd_draw_textf(font, 20, 116, RGBA8(77, 76, 74, 255), 12, "Model: %s %s", getModel(), getRegion());
	sftd_draw_textf(font, 20, 132, RGBA8(77, 76, 74, 255), 12, "Language: %s", getLang());
	sftd_draw_textf(font, 20, 148, RGBA8(77, 76, 74, 255), 12, "ECS Device ID: %llu", getSoapId());
	sftd_draw_textf(font, 20, 164, RGBA8(77, 76, 74, 255), 12, "Friend Code: %llu", principalIdToFriendCode(getMyFriendKey().principalId));
	sftd_draw_textf(font, 20, 180, RGBA8(77, 76, 74, 255), 12, "MAC Address: %s", getMacAddress());
	sftd_draw_textf(font, 20, 196, RGBA8(77, 76, 74, 255), 12, "Serial number: %s", getSerialNum());
	sftd_draw_textf(font, 20, 212, RGBA8(77, 76, 74, 255), 12, "Screen type: %s", getScreenType());
}

void batteryMenu()
{
	u8 batteryPercent, batteryVolt;
	
	sftd_draw_textf(font, 165, 100, RGBA8(0, 0, 0, 255), 12, "Battery Menu");
	
	mcuGetBatteryLevel(&batteryPercent);
	mcuGetBatteryVoltage(&batteryVolt);
	
	sftd_draw_textf(font, 20, 116, RGBA8(77, 76, 74, 255), 12, "Battery percentage: %3d%%", batteryPercent);
	sftd_draw_textf(font, 20, 132, RGBA8(77, 76, 74, 255), 12, "Battery status: %s", batteryStatus());
	sftd_draw_textf(font, 20, 148, RGBA8(77, 76, 74, 255), 12, "Battery voltage: %d", batteryVolt);
}

void miscMenu()
{
	FS_ArchiveResource	resource = {0};
	u8 volume;
	u32 installedTitles = titleCount(MEDIATYPE_SD);
	
	double wifiPercent = (osGetWifiStrength() * 33.3333333333);
	
	mcuGetVolume(&volume);
	double volPercent = (volume * 1.5873015873);
		
	double _3dSliderPercent = (osGet3DSliderState() * 100.0);
	
	sftd_draw_textf(font, 165, 100, RGBA8(0, 0, 0, 255), 12, "Miscelleanous");
	
	sftd_draw_textf(font, 20, 116, RGBA8(77, 76, 74, 255), 12, "SD Size: %.1f MB / %.1f MB",
				(((u64) resource.freeClusters * (u64) resource.clusterSize) / 1024.0 / 1024.0),
				(((u64) resource.totalClusters * (u64) resource.clusterSize) / 1024.0 / 1024.0));
				
	sftd_draw_textf(font, 20, 132, RGBA8(77, 76, 74, 255), 12, "CTR Size: %.1f MB / %.1f MB",
				(((u64) resource.freeClusters * (u64) resource.clusterSize) / 1024.0 / 1024.0),
				(((u64) resource.totalClusters * (u64) resource.clusterSize) / 1024.0 / 1024.0));
				
	sftd_draw_textf(font, 20, 148, RGBA8(77, 76, 74, 255), 12, "Installed titles: %i", (int)installedTitles);
	sftd_draw_textf(font, 20, 164, RGBA8(77, 76, 74, 255), 12, "WiFi signal strength: %d (%.0lf%%)", osGetWifiStrength(), wifiPercent);
	sftd_draw_textf(font, 20, 180, RGBA8(77, 76, 74, 255), 12, "Volume slider state: %d (%.0lf%%)", volume, volPercent);
	sftd_draw_textf(font, 20, 196, RGBA8(77, 76, 74, 255), 12, "3D slider state: %.1lf (%.0lf%%)", osGet3DSliderState(), _3dSliderPercent);
}

int main(int argc, char *argv[])
{      
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
	psInit();

	sf2d_init();
	sftd_init();
	
	sf2d_set_clear_color(RGBA8(0, 0, 0, 255));
	sf2d_set_vblank_wait(0);
	
	load_PNG(topScreen, "/3ds/3DSident/res/topScreen.png");
	load_PNG(bottomScreen, "/3ds/3DSident/res/bottomScreen.png");
	font = sftd_load_font_mem(Ubuntu_ttf, Ubuntu_ttf_size);
	
	int MenuSelection = 1; // Pretty obvious
	int selector_x = 16; //The x position of the first selection
	int selector_y = 17; //The y position of the first selection
	int numMenuItems = 5; //Amount of items in the menu
	int selector_image_x = 0; //Determines the starting x position of the selection
	int selector_image_y = 0; //Determines the starting y position of the selection
	
    while (aptMainLoop())
    {
		selector_image_x = selector_x + (selector_xDistance * MenuSelection); //Determines where the selection image is drawn for each selection
        selector_image_y = selector_y + (selector_yDistance * MenuSelection); //Determines where the selection image is drawn for each selection
		
		hidScanInput();
		u32 kDown = hidKeysDown();
		
		sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		
		sf2d_draw_texture(bottomScreen, 0, 0);
		sf2d_draw_rectangle(selector_image_x, selector_image_y, 286, 18, RGBA8(242, 119, 62, 255));
		
		sftd_draw_textf(font, 22, 37, RGBA8(78, 74, 67, 255), 12, "Kernel Information");
		
		sftd_draw_textf(font, 22, 55, RGBA8(78, 74, 67, 255), 12, "System Information");
		
		sftd_draw_textf(font, 22, 73, RGBA8(78, 74, 67, 255), 12, "Battery Information");
		
		sftd_draw_textf(font, 22, 91, RGBA8(78, 74, 67, 255), 12, "Miscelleanous");
		
		sftd_draw_textf(font, 22, 109, RGBA8(78, 74, 67, 255), 12, "Exit");
		
		if (kDown & KEY_DOWN) 
			MenuSelection++; //Moves the selector down
        if (kDown & KEY_UP) 
			MenuSelection--; //Moves the selector up
        
        if (MenuSelection > numMenuItems) 
			MenuSelection = 1; //Sets the selection to the first
        if (MenuSelection < 1) 
			MenuSelection = numMenuItems; //Sets the selection back to last
		
		sf2d_end_frame();
		
        sf2d_start_frame(GFX_TOP, GFX_LEFT);
        		
		sf2d_draw_texture(topScreen, 0, 0);
		
		if (MenuSelection == 1)
			kernelMenu();
		else if (MenuSelection == 2)
			systemMenu();
		else if (MenuSelection == 3)
			batteryMenu();
		else if (MenuSelection == 4)
			miscMenu();
		else if ((MenuSelection == 5) && (kDown & KEY_A))
			break;
		
		if ((kDown & KEY_L) && (kDown & KEY_R))
			captureScreenshot();
		
		sf2d_end_frame();
		
		// Flush and swap framebuffers
		sf2d_swapbuffers();
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
    return 0;
}
