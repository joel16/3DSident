#include "cfgs.h"

static Handle cfgHandle;

Result cfgsInit() //Already initialized with CFGUinit();
{
    return srvGetServiceHandle(&cfgHandle, "cfg:s");
}

Result cfgsExit()
{
    return svcCloseHandle(cfgHandle);
}

Result CFG_GetConfig(u32 size, u32 blkID, u8 * outData)
{
	Result ret = 0;
	u32 *cmdbuf = getThreadCommandBuffer();

	cmdbuf[0] = IPC_MakeHeader(0x1, 2, 2); // 0x10082
	cmdbuf[1] = size;
	cmdbuf[2] = blkID;
	cmdbuf[3] = IPC_Desc_Buffer(size,IPC_BUFFER_W);
	cmdbuf[4] = (u32)outData;

	if (R_FAILED(ret = svcSendSyncRequest(cfgHandle)))
		return ret;

	return (Result)cmdbuf[1];
}

Result cfgsSecureInfoGetSerialNo(char * serial)
{
	Result ret = 0;
	u32 *cmdbuf = getThreadCommandBuffer();

	cmdbuf[0] = IPC_MakeHeader(0x408, 1, 2); // 0x10082
	cmdbuf[1] = 0xF;
	cmdbuf[2] = 12 | (0xF << 4);
	cmdbuf[3] = (u32)serial;

	if (R_FAILED(ret = svcSendSyncRequest(cfgHandle)))
		return ret;

	return (Result)cmdbuf[1];
}