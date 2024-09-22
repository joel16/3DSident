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
    bool slot[3];
    const char *ssid[3];
    const char *passphrase[3];
    const char *securityMode[3];
} WifiInfo;

typedef struct {
    u64 usedSize[4];
    u64 totalSize[4];
    char freeSizeString[4][16];
    char usedSizeString[4][16];
    char totalSizeString[4][16];
} StorageInfo;

typedef struct {
    u32 sdTitleCount;
    u32 nandTitleCount;
    u32 ticketCount;
} MiscInfo;

namespace ACI {
    Result GetSSID(char *ssid);
    Result GetSecurityMode(acSecurityMode *mode);
    Result GetPassphrase(char *passphrase);
}

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
    WifiInfo GetWifiInfo(void);
    StorageInfo GetStorageInfo(void);
    MiscInfo GetMiscInfo(void);
}
