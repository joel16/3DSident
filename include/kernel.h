#pragma once

typedef enum {
    VERSION_INFO_KERNEL = 0,
    VERSION_INFO_FIRM,
    VERSION_INFO_SYSTEM
} VersionInfo;

namespace Kernel {
    const char *GetInitalVersion(void);
    const char *GetVersion(VersionInfo info);
    const char *GetSdmcCid(void);
    const char *GetNandCid(void);
    u64 GetDeviceId(void);
}
