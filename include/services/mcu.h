#ifndef MCU_H
#define MCU_H

#include <3ds.h>

Result mcuInit(void);
Result mcuExit(void);
Result MCU_ReadRegister(u8 reg, u32 size, void * data);
Result MCU_GetBatteryVoltage(u8 * out);
Result MCU_GetBatteryLevel(u8 * out);
Result MCU_GetSoundVolume(u8 * out);
Result MCU_GetFwVerHigh(u8 * out);
Result MCU_GetFwVerLow(u8 * out);

#endif