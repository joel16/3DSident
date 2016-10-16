#include "am.h"

Result AMNet_GetDeviceCert(u8 *buffer)
{
	Result ret = 0;
	u32 *cmdbuf = getThreadCommandBuffer();

	cmdbuf[0] = IPC_MakeHeader(0x818,1,2); // 0x08180042
	cmdbuf[1] = 0x180;
	cmdbuf[2] = (0x180 << 4) | 0xC;
	cmdbuf[3] = (u32)buffer;

	if(R_FAILED(ret = svcSendSyncRequest(amHandle))) return ret;

	return (Result)cmdbuf[1];
}
