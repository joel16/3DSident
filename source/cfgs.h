#include <3ds.h>

Handle cfgHandle;

Result cfgsInit();
Result cfgsExit();
Result CFG_GetConfig(u32 size, u32 blkID, u8* outData);
Result cfgsSecureInfoGetSerialNo(char *serial);