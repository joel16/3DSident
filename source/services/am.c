#include "am.h"

static Handle amHandle;

Result amGetServiceHandle(void)
{
	return srvGetServiceHandle(&amHandle, "am:net");
}

Result amCloseServiceHandle(void)
{
	return svcCloseHandle(amHandle);
}

Result amNetGetDeviceCert(u8 const * buffer)
{
	Result ret = 0;
	u32 *cmdbuf = getThreadCommandBuffer();

	cmdbuf[0] = IPC_MakeHeader(0x818, 1, 2); // 0x08180042
	cmdbuf[1] = 0x180;
	cmdbuf[2] = (0x180 << 4) | 0xC;
	cmdbuf[3] = (u32)buffer;

	if(R_FAILED(ret = svcSendSyncRequest(amHandle))) 
		return ret;

	return (Result)cmdbuf[1];
}
