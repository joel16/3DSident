#include "actu.h"

static Handle actHandle;
static int actRefCount;

static u32 *act_shareMemAddr;
static u32 act_shareMemSize;
static Handle act_shareMemHandle;

Result actInit(u32 sdkVer, u32 sharedMemSize)
{
    Result ret = 0;

	ret = srvGetServiceHandle(&actHandle, "act:u");
	
	if (R_FAILED(ret)) 
		ret = srvGetServiceHandle(&actHandle, "act:a");
	
	if (R_SUCCEEDED(ret))
	{
		act_shareMemSize = sharedMemSize;
		act_shareMemHandle = 0;

		if(act_shareMemSize)
		{
			act_shareMemAddr = (u32*)memalign(0x1000, act_shareMemSize);
			
			if(act_shareMemAddr != NULL)
			{
				memset((void*)act_shareMemAddr, 0, act_shareMemSize);
				ret = svcCreateMemoryBlock(&act_shareMemHandle, (u32)act_shareMemAddr, act_shareMemSize, 0, MEMPERM_READ | MEMPERM_WRITE);
                if(R_FAILED(ret))
                    actExit();
			}
		}
		
        ret = ACT_Initialize(sdkVer, &act_shareMemHandle, act_shareMemSize);
        
		if (R_FAILED(ret))
            actExit();
    }
	return ret;
}

void actExit(void)
{
	if (AtomicDecrement(&actRefCount)) 
		return;
	
	svcCloseHandle(actHandle);

	if(act_shareMemHandle)
	{
        svcUnmapMemoryBlock(act_shareMemHandle, (u32)act_shareMemAddr);
        free(act_shareMemAddr);
        act_shareMemAddr = NULL;
        svcCloseHandle(act_shareMemHandle);
		act_shareMemHandle = 0;
		act_shareMemSize = 0;
	}
}

Result ACT_Initialize(u32 sdkVer, void *addr, u32 memSize)
{
	Result ret = 0;
	u32 *cmdbuf = getThreadCommandBuffer();

	cmdbuf[0] = IPC_MakeHeader(0x1,2,4); // 0x00010084
    cmdbuf[1] = sdkVer;
    cmdbuf[2] = memSize;
    cmdbuf[3] = 0x20;
    cmdbuf[4] = IPC_Desc_CurProcessHandle();
    cmdbuf[5] = 0;
    cmdbuf[6] = (u32)addr;

    if(R_FAILED(ret = svcSendSyncRequest(actHandle))) 
		return ret;
	
	return (Result)cmdbuf[1];
}

Result ACT_GetAccountInfo(void *buffer, u32 size, u32 blkId)
{
    Result ret = 0;
	u32 *cmdbuf = getThreadCommandBuffer();

	cmdbuf[0] = IPC_MakeHeader(6,3,2); // 0x00600C2
    cmdbuf[1] = 0xFE;
    cmdbuf[2] = size;
    cmdbuf[3] = blkId;
    cmdbuf[4] = 0x10 * size | 0xC;
    cmdbuf[5] = (u32)buffer;
    
    if(R_FAILED(ret = svcSendSyncRequest(actHandle))) 
		return ret;
	
	return (Result)cmdbuf[1];
}

Result actuInit(void) 
{
    return srvGetServiceHandle(&actHandle, "act:u");
}

Result actuExit(void) 
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

    if((ret = svcSendSyncRequest(actHandle))!= 0) 
		return ret;

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

    if((ret = svcSendSyncRequest(actHandle))!= 0) 
		return ret;

    return (Result)cmdbuf[1];
}
