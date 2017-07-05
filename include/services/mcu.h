#ifndef MCU_H
#define MCU_H

#include <3ds.h>

Result mcuInit();
Result mcuExit();
Result mcuGetBatteryLevel(u8* out);
Result mcuGetBatteryVoltage(u8* out);
Result GetMcuFwVerHigh(u8* out);
Result GetMcuFwVerLow(u8* out);

#endif