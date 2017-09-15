#include "power.h"
#include "screenshot.h"

const char * batteryStatus(void)
{
    u8 batteryStatus;
    
	if (R_SUCCEEDED(PTMU_GetBatteryChargeState(&batteryStatus)))
	{
		if (batteryStatus)
			return "charging";
		
		return "not charging";
	}
	
	return NULL;
}