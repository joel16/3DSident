#include "gsplcd.h"

static Handle gspLcdHandle;

Result GSPLCD_GetBrightness(u32 screen, u32 *brightness)
{
    u32 *cmdbuf = getThreadCommandBuffer();
 
    cmdbuf[0] = IPC_MakeHeader(0x15,1,0); // 0x150040
    cmdbuf[1] = screen;
 
    Result ret = 0;
    if (R_FAILED(ret = svcSendSyncRequest(gspLcdHandle))) return ret;
   
    *brightness = cmdbuf[2];
   
    return cmdbuf[2];
}