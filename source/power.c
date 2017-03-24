#include "power.h"
#include "screenshot.h"

const char * batteryStatus()
{
    u8 batteryStateBool;
    PTMU_GetBatteryChargeState(&batteryStateBool);

    if (!batteryStateBool) 
        return "not charging";
    else 
        return "charging";
}