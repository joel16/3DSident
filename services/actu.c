#include <malloc.h> 
#include <stdio.h>
#include <string.h>

#include "actu.h"

static Handle actHandle;
static int actRefCount;

Result actInit(void)
{
	Result ret = 0;

	if (AtomicPostIncrement(&actRefCount)) 	
		return 0;
	
	ret = srvGetServiceHandle(&actHandle, "act:u");
	
	if (R_FAILED(ret)) 
		ret = srvGetServiceHandle(&actHandle, "act:a");
	
	if (R_FAILED(ret)) 
		AtomicDecrement(&actRefCount);

	return ret;
}

void actExit(void)
{
	if (AtomicDecrement(&actRefCount)) 
		return;
	
	svcCloseHandle(actHandle);
}

Result ACTU_GetAccountDataBlock(void * buffer, u32 size, u32 blkId)
{
	Result ret = 0;
	u32 *cmdbuf = getThreadCommandBuffer();

	cmdbuf[0] = IPC_MakeHeader(6, 3, 2); // 0x00600C2
	cmdbuf[1] = 0xFE;
	cmdbuf[2] = size;
	cmdbuf[3] = blkId;
	cmdbuf[4] = 0x10 * size | 0xC;
	cmdbuf[5] = (u32)buffer;
    
	if (R_FAILED(ret = svcSendSyncRequest(actHandle))) 
		return ret;
	
	return (Result)cmdbuf[1];
}