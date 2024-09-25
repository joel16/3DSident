#include <3ds.h>
#include <cstdio>
#include <memory>

#include "system.h"

namespace System {
    const char *GetModel(void) {
        const char *models[] = {
            "OLD 3DS - CTR",
            "OLD 3DS XL - SPR",
            "NEW 3DS - KTR",
            "OLD 2DS - FTR",
            "NEW 3DS XL - RED",
            "NEW 2DS XL - JAN"
        };

        u8 model = 0;
        if (R_FAILED(CFGU_GetSystemModel(std::addressof(model)))) {
            return "unknown";
        }
        
        return models[model];
    }

    const char *GetRegion(void) {
        const char *regions[] = {
            "JPN",
            "USA",
            "EUR",
            "AUS",
            "CHN",
            "KOR",
            "TWN"
        };
        
        u8 region = 0;
        if (R_FAILED(CFGU_SecureInfoGetRegion(std::addressof(region)))) {
            return "unknown";
        }

        return regions[region];
    }

    const char *GetFirmRegion(void) {
        const char *regions[] = {
            "J",
            "U",
            "E",
            "E",
            "C",
            "K",
            "T"
        };

        u8 region = 0;
        if (R_FAILED(CFGU_SecureInfoGetRegion(std::addressof(region)))) {
            return "unknown";
        }

        return regions[region];
    }

    bool IsCoppacsSupported(void) {
        u8 isCoppacs = 0;

        if (R_FAILED(CFGU_GetRegionCanadaUSA(std::addressof(isCoppacs)))) {
            return false;
        }

        return isCoppacs? true : false;
    }

    const char *GetLanguage(void) {
        const char *languages[] = {
            "Japanese",
            "English",
            "French",
            "German",
            "Italian",
            "Spanish",
            "Simplified Chinese",
            "Korean",
            "Dutch",
            "Portugese",
            "Russian",
            "Traditional Chinese"
        };

        u8 language = 0;
        if (R_FAILED(CFGU_GetSystemLanguage(std::addressof(language)))) {
            return "unknown";
        }

        return languages[language];
    }

    const char *GetMacAddress(void) {
        u8 *addr = OS_SharedConfig->wifi_macaddr;
        static char macAddress[0x12];
        std::snprintf(macAddress, 0x12, "%02X:%02X:%02X:%02X:%02X:%02X", *addr, *(addr + 1), *(addr + 2), *(addr + 3), *(addr + 4), *(addr + 5));
        return macAddress;
    }

    const char *GetRunningHW(void) {
        const char *runningHW[] = {
            "unknown",
            "product",
            "TS board",
            "KMC debugger",
            "KMC capture",
            "IS debugger",
            "snake product",
            "snake IS debugger",
            "snake IS capture",
            "snake KMC debugger"
        };

        return runningHW[(OS_SharedConfig->running_hw) - 1];
    }

    const char *IsDebugUnit(void) {
        return OS_KernelConfig->unit_info? "" : "(Debug Unit)";
    }

    u64 GetLocalFriendCodeSeed(void) {
        Result ret = 0;
        u64 seed = 0;

        if (R_FAILED(ret = CFGI_GetLocalFriendCodeSeed(std::addressof(seed)))) {
            return ret;
        }

        return seed;
    }
    
    u8 *GetSerialNumber(void) {
        static u8 serial[0xF];
        
        if (R_FAILED(CFGI_SecureInfoGetSerialNumber(serial))) {
            return nullptr;
        }
        
        return serial;
    }

    u64 GetDeviceId(void) {
        Result ret = 0;
        u32 id = 0;

        if (R_FAILED(ret = AM_GetDeviceId(std::addressof(id)))) {
            return ret;
        }
        
        return (id | (((u64) 4) << 32));
    }
}
