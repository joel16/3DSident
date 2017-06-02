#include <3ds.h>
#include <string.h>

Handle frdHandle;

typedef struct
{
   u32 principalId;
   u64 localFriendCode;
} FriendKey;

typedef struct
{
   u8 region;
   u8 country;
   u8 area;
   u8 language;
   u8 platform;
} FrdProfile;

Result frdInit();
Result frdExit();
Result frdSetClientSdkVersion(u32 sdkVer);
u64 frdPrincipalIdToFriendCode(u64 *fc, u64 pid);
Result frdGetMyFriendKey(FriendKey *key);