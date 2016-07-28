#include <3ds.h>
#include <stdio.h>

Result actInit(void);
Result actExit(void);
Result ACTU_Initialize(u32 sdkVersion, u32 unknown, Handle handle);
Result ACTU_GetAccountDataBlock(u32 unknown, u32 size, u32 blockId, void* output);