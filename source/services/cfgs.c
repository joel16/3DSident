#include "cfgs.h"

static Handle cfgsHandle;

Result cfgsInit(void) // Already initialized with CFGUinit(), but we need to use this handle.
{
    return srvGetServiceHandle(&cfgsHandle, "cfg:s");
}

Result cfgsExit(void)
{
    return svcCloseHandle(cfgsHandle);
}

Result CFGS_SecureInfoGetSerialNo(u8 * serial)
{
	Result ret = 0;
	u32 * cmdbuf = getThreadCommandBuffer();

	cmdbuf[0] = IPC_MakeHeader(0x408, 1, 2); // 0x04080042
	cmdbuf[1] = 0xF;
	cmdbuf[2] = 12 | (0xF << 4);
	cmdbuf[3] = (u32)serial;

	if (R_FAILED(ret = svcSendSyncRequest(cfgsHandle)))
		return ret;

	return cmdbuf[1];
}