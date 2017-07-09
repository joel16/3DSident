#ifndef ACTU_H
#define ACTU_H

#include <3ds.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>

Result actInit(u32 sdkVer, u32 sharedMemSize);
void actExit(void);
Result ACTU_Initialize(u32 sdkVer, void *addr, u32 memSize);
Result ACTU_GetAccountDataBlock(void *buffer, u32 size, u32 blkId);

#endif