#ifndef SYSTEM_H
#define SYSTEM_H

#include <3ds.h>

const char * getModel();
const char * getRegion();
const char * getLang();
char * getMacAddress();
char * getScreenType();
u64 getLocalFriendCodeSeed();
char * getSerialNum(void);
u32 getDeviceId(void);
u64 getSoapId(void);
char * getDeviceCert(void);
char * getNNID(void);
char * isDebugModeEnabled();
char * getBrightness(u32 screen);
char * getCardSlotStatus();

#endif