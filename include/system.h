#ifndef SYSTEM_H
#define SYSTEM_H

#include <3ds.h>

const char * getModel(void);
const char * getRegion(void);
const char getFirmRegion(void);
const char * getLang(void);
char * getMacAddress(void);
char * getScreenType(void);
u64 getLocalFriendCodeSeed(void);
char * getSerialNum(void);
u32 getDeviceId(void);
u64 getSoapId(void);
char * getDeviceCert(void);
char * getNNIDInfo(u32 size, u32 blkId);
char * getBrightness(u32 screen);
char * getCardSlotStatus(void);

#endif