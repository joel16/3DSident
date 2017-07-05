#ifndef ACTU_H
#define ACTU_H

#include <3ds.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>

Result actInit(u32 sdkVer, u32 sharedMemSize);
void actExit(void);
Result ACT_Initialize(u32 sdkVer, void *addr, u32 memSize);
Result ACT_GetAccountInfo(void *buffer, u32 size, u32 blkId);
Result actuInit(void);
Result actuExit(void);
Result ACTU_Initialize(u32 sdkVersion, u32 unknown, Handle handle);
Result ACTU_GetAccountDataBlock(u32 unknown, u32 size, u32 blockId, void* output);

#endif