#include "cfgs.h"

Result cfgsInit() //Already initialized with CFGUinit();
{
    return srvGetServiceHandle(&cfgHandle, "cfg:s");
}

Result cfgsExit()
{
    return svcCloseHandle(cfgHandle);
}

Result cfgsSecureInfoGetSerialNo(char *serial)
{
	Result ret = 0;
	u32 *cmdbuf = getThreadCommandBuffer();

	cmdbuf[0] = IPC_MakeHeader(0x408,1,2); // 0x10082
	cmdbuf[1] = 0xF;
	cmdbuf[2] = 12 | (0xF << 4);
	cmdbuf[3] = (u32)serial;

	if(R_FAILED(ret = svcSendSyncRequest(cfgHandle)))return ret;

	return (Result)cmdbuf[1];
}