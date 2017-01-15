#include <3ds.h>

const char * getModel();
const char * getRegion();
const char * getLang();
char * getMacAddress();
char * getScreenType();
u64 principalIdToFriendCode(u64 pid);
u64 getLocalFriendCodeSeed();
FriendKey getMyFriendKey(void);
char * getSerialNum(void);
u32 getDeviceId(void);
u64 getSoapId(void);
char * getDeviceCert(void);
char * getNNID(void);