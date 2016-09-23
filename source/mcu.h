#include <3ds.h>

Handle mcuhwcHandle;

Result mcuInit();
Result mcuExit();
Result mcuGetBatteryLevel(u8* out);