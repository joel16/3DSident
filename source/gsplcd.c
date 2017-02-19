#include "gsplcd.h"

Result GSPLCD_GetBrightness(u32 screen) //Screen index top = 1, bottom = 2 
{
	u32 *cmdbuf = getThreadCommandBuffer();

	cmdbuf[0] = IPC_MakeHeader(0x15,1,0); // 0x150040
	cmdbuf[1] = screen;

	Result ret = 0;
	if (R_FAILED(ret = svcSendSyncRequest(gspLcdHandle))) return ret;
	
	screen = cmdbuf[2];
	
	return cmdbuf[1];
}