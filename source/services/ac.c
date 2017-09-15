#include "ac.h"

static Handle acHandle;

Result acGetServiceHandle(void)
{
	return srvGetServiceHandle(&acHandle, "ac:u");
}

Result acCloseServiceHandle(void)
{
	return svcCloseHandle(acHandle);
}

Result ACU_GetConnectingSecurityMode(u32 * out)
{
	Result ret = 0;
	u32 * cmdbuf = getThreadCommandBuffer();

	cmdbuf[0] = IPC_MakeHeader(0x33, 0, 0); // 0x00330000

	if (R_FAILED(ret = svcSendSyncRequest(acHandle)))
		return ret;

	*out = cmdbuf[2];

	return cmdbuf[1];
}

Result ACU_GetConnectingSsid(u32 * out)
{
	Result ret = 0;
	u32 * cmdbuf = getThreadCommandBuffer();

	cmdbuf[0] = IPC_MakeHeader(0x34, 0, 0); // 0x00340000

	if (R_FAILED(ret = svcSendSyncRequest(acHandle)))
		return ret;

	*out = cmdbuf[2];

	return cmdbuf[1];
}

Result ACU_GetConnectingSsidLength(u32 * out)
{
	Result ret = 0;
	u32 * cmdbuf = getThreadCommandBuffer();

	cmdbuf[0] = IPC_MakeHeader(0x35, 0, 0); // 0x00350000

	if (R_FAILED(ret = svcSendSyncRequest(acHandle)))
		return ret;

	*out = cmdbuf[2];

	return cmdbuf[1];
}