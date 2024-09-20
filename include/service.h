#pragma once

#include <3ds.h>

struct Birthday {
    u16 year; 
    u8 month; 
    u8 day;
};

typedef struct {
    const char *kernelVersion;
    const char *firmVersion;
    const char *systemVersion;
    const char *initialVersion;
    const char *sdmcCid;
    const char *nandCid;
    u64 deviceId;
} KernelInfo;

typedef struct {
    const char *model;
    const char *hardware;
    const char *region;
    const char *language;
    u64 localFriendCodeSeed;
    const char *macAddress;
    u8 *serialNumber;
} SystemInfo;

typedef struct {
    u32 principalID;
    u32 persistentID;
    u64 transferableID;
    char country[0x3];
    char nnid[0x11];
    char timezone[0x41];
} NNIDInfo;

typedef struct {
    const char *username;
    const char *birthday;
    const char *eulaVersion;
    const char *parentalPin;
    const char *parentalEmail;
    const char *parentalSecretAnswer;
} ConfigInfo;

typedef struct {
    const char *screenUpper;
    const char *screenLower;
    const char *soundOutputMode;
} HardwareInfo;

typedef struct {
    u32 sdTitleCount;
    u32 nandTitleCount;
    u32 ticketCount;
} MiscInfo;

typedef struct {
    u64 sdUsed;
    u64 sdTotal;
    u64 ctrUsed;
    u64 ctrTotal;
    u64 twlUsed;
    u64 twlTotal;
    u64 twlpUsed;
    u64 twlpTotal;
    char sdFreeSize[16];
    char sdUsedSize[16];
    char sdTotalSize[16];
    char ctrFreeSize[16];
    char ctrUsedSize[16];
    char ctrTotalSize[16];
    char twlFreeSize[16];
    char twlUsedSize[16];
    char twlTotalSize[16];
    char twlpFreeSize[16];
    char twlpUsedSize[16];
    char twlpTotalSize[16];
} StorageInfo;

namespace MCUHWC {
    Result GetBatteryTemperature(u8 *temp);
}

namespace Service {
    void Init(void);
    void Exit(void);
    
    KernelInfo GetKernelInfo(void);
	SystemInfo GetSystemInfo(void);
    ConfigInfo GetConfigInfo(void);
    HardwareInfo GetHardwareInfo(void);
    StorageInfo GetStorageInfo(void);
    MiscInfo GetMiscInfo(void);
}
