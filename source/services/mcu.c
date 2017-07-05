#include "mcu.h"

static Handle mcuhwcHandle;

Result mcuInit()
{
    return srvGetServiceHandle(&mcuhwcHandle, "mcu::HWC");
}

Result mcuExit()
{
    return svcCloseHandle(mcuhwcHandle);
}

Result mcuGetBatteryLevel(u8* out)
{
	u32* ipc = getThreadCommandBuffer();
	ipc[0] = 0x50000;
   
	Result ret = svcSendSyncRequest(mcuhwcHandle);
	
	if(ret < 0) 
		return ret;
	
	*out = ipc[2];
	
	return ipc[1];
}

Result mcuGetBatteryVoltage(u8* out)
{
	u32* ipc = getThreadCommandBuffer();
	ipc[0] = 0x40000;
    
	Result ret = svcSendSyncRequest(mcuhwcHandle);
	
	if(ret < 0) 
		return ret;
	
	*out = ipc[2];
	
	return ipc[1];
}

Result GetMcuFwVerHigh(u8* out) 
{
	u32* ipc = getThreadCommandBuffer();
	ipc[0] = 0x100000;
	
	Result ret = svcSendSyncRequest(mcuhwcHandle);
	
	if(ret < 0) 
		return ret;
	
	*out = ipc[2];
	
	return ipc[1];
}

Result GetMcuFwVerLow(u8* out) 
{
	u32* ipc = getThreadCommandBuffer();
	ipc[0] = 0x110000;
	
	Result ret = svcSendSyncRequest(mcuhwcHandle);
	
	if(ret < 0) 
		return ret;
		
	*out = ipc[2];
	
	return ipc[1];
}