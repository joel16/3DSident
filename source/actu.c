#include <3ds.h>
#include <stdio.h>

static Handle actHandle;

Result actInit(void) 
{
    return srvGetServiceHandle(&actHandle, "act:u");
}

Result actExit(void) 
{
    return svcCloseHandle(actHandle);
}

Result ACTU_Initialize(u32 sdkVersion, u32 unknown, Handle handle) 
{
    Result ret = 0;
    u32 *cmdbuf = getThreadCommandBuffer();

    cmdbuf[0] = 0x00010084;
    cmdbuf[1] = sdkVersion;
    cmdbuf[2] = unknown;
    cmdbuf[3] = 0x20;
    cmdbuf[4] = 0;
    cmdbuf[5] = 0x0;
    cmdbuf[6] = handle;

    if((ret = svcSendSyncRequest(actHandle))!=0) return ret;

    return (Result)cmdbuf[1];
}

Result ACTU_GetAccountDataBlock(u32 unknown, u32 size, u32 blockId, void* output) 
{
    Result ret = 0;
    u32 *cmdbuf = getThreadCommandBuffer();

    cmdbuf[0] = 0x000600C2;
    cmdbuf[1] = unknown;
    cmdbuf[2] = size;
    cmdbuf[3] = blockId;
    cmdbuf[4] = (size << 4) | 0xC;
    cmdbuf[5] = (u32) output;

    if((ret = svcSendSyncRequest(actHandle))!=0) return ret;

    return (Result)cmdbuf[1];
}
