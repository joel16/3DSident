#include <3ds.h>
#include <malloc.h>

#include "ac.h"
#include "actu.h"
#include "C2D_helper.h"
#include "common.h"
#include "fs.h"
#include "menus.h"
#include "sprites.h"
#include "utils.h"

static u32 cpu_time_limit = 0;
static C2D_SpriteSheet spritesheet;

static void Init_Services(void)
{
	aciInit();
	
	// Check if user is running from *hax.
	//if (envIsHomebrew() && (R_FAILED(actInit())))
	//	isHomebrew = envIsHomebrew();
	
	actInit();
	
	amAppInit();
	amInit();
	cfguInit();
	dspInit();
	mcuHwcInit();
	ptmuInit();
	socInit((u32*)memalign(0x1000, 0x10000), 0x10000);

	FS_OpenArchive(&archive, ARCHIVE_SDMC);
	
	romfsInit();
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();

	if (Utils_IsN3DS())
		osSetSpeedupEnable(true);

	APT_GetAppCpuTimeLimit(&cpu_time_limit);
	APT_SetAppCpuTimeLimit(30);

	staticBuf = C2D_TextBufNew(4096);
	dynamicBuf = C2D_TextBufNew(4096);
	sizeBuf = C2D_TextBufNew(4096);

	RENDER_TOP = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	RENDER_BOTTOM = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

	spritesheet = C2D_SpriteSheetLoad("romfs:/res/drawable/sprites.t3x");
	banner = C2D_SpriteSheetGetImage(spritesheet, sprites_icon_idx);
	drive_icon = C2D_SpriteSheetGetImage(spritesheet, sprites_drive_idx);
}

static void Term_Services(void)
{
	C2D_SpriteSheetFree(spritesheet);

	C2D_TextBufDelete(sizeBuf);
	C2D_TextBufDelete(dynamicBuf);
	C2D_TextBufDelete(staticBuf);

	if (cpu_time_limit != UINT32_MAX)
		APT_SetAppCpuTimeLimit(cpu_time_limit);

	if (Utils_IsN3DS())
		osSetSpeedupEnable(false);
	
	FS_CloseArchive(archive);
	
	socExit();
	ptmuExit();
	mcuHwcExit();
	dspExit();
	cfguExit();
	amExit();
	actExit();
	acExit();
	aciExit();
}

int main(int argc, char **argv) 
{
	Init_Services();

	if (setjmp(exitJmp)) 
	{
		Term_Services();
		return 0;
	}

	Menu_Main();

	Term_Services();
	return 0;
}
