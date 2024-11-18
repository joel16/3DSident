#include <3ds.h>
#include <cstdio>
#include <memory>

#include "log.h"
#include "system.h"

namespace System {
    const char *GetModel(void) {
        Result ret = 0;
        const char *models[] = {
            "OLD 3DS - CTR",
            "OLD 3DS XL - SPR",
            "NEW 3DS - KTR",
            "OLD 2DS - FTR",
            "NEW 3DS XL - RED",
            "NEW 2DS XL - JAN"
        };

        u8 model = 0;
        if (R_FAILED(ret = CFGU_GetSystemModel(std::addressof(model)))) {
            Log::Error("%s failed: 0x%x\n", __func__, ret);
            return "unknown";
        }
        
        return models[model];
    }

    const char *GetRegion(void) {
        Result ret = 0;
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
        if (R_FAILED(ret = CFGU_SecureInfoGetRegion(std::addressof(region)))) {
            Log::Error("%s failed: 0x%x\n", __func__, ret);
            return "unknown";
        }

        return regions[region];
    }

    const char *GetFirmRegion(void) {
        Result ret = 0;
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
        if (R_FAILED(ret = CFGU_SecureInfoGetRegion(std::addressof(region)))) {
            Log::Error("%s failed: 0x%x\n", __func__, ret);
            return "unknown";
        }

        return regions[region];
    }

    bool IsCoppacsSupported(void) {
        Result ret = 0;
        u8 isCoppacs = 0;

        if (R_FAILED(ret = CFGU_GetRegionCanadaUSA(std::addressof(isCoppacs)))) {
            Log::Error("%s failed: 0x%x\n", __func__, ret);
            return false;
        }

        return isCoppacs? true : false;
    }

    const char *GetLanguage(void) {
        Result ret = 0;
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
        if (R_FAILED(ret = CFGU_GetSystemLanguage(std::addressof(language)))) {
            Log::Error("%s failed: 0x%x\n", __func__, ret);
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

        return runningHW[OS_SharedConfig->running_hw];
    }

    const char *IsDebugUnit(void) {
        return OS_KernelConfig->unit_info? "" : "(Debug Unit)";
    }

    u64 GetLocalFriendCodeSeed(void) {
        Result ret = 0;
        u64 seed = 0;

        if (R_FAILED(ret = CFGI_GetLocalFriendCodeSeed(std::addressof(seed)))) {
            Log::Error("%s failed: 0x%x\n", __func__, ret);
            return ret;
        }

        return seed;
    }
    
    u8 *GetSerialNumber(void) {
        Result ret = 0;
        static u8 serial[15];
        
        if (R_FAILED(ret = CFGI_SecureInfoGetSerialNumber(serial))) {
            Log::Error("%s failed: 0x%x\n", __func__, ret);
            return nullptr;
        }
        
        return serial;
    }
    
    int GetCheckDigit(const u8* serialNumber) {
        int oddSum = 0;
        int evenSum = 0;
        
        for (size_t i = 2; i < 10; ++i) {
            int digit = serialNumber[i] - '0';
            if (i % 2 == 0) {
                oddSum += digit;
            }
            else {
                evenSum += digit;
            }
        }
        
        int checkDigit = ((3 * evenSum) + oddSum) % 10;
        return (checkDigit == 0) ? checkDigit : 10 - checkDigit;
    }

    u64 GetSoapId(void) {
        Result ret = 0;
        u32 id = 0;

        if (R_FAILED(ret = AM_GetDeviceId(std::addressof(id)))) {
            Log::Error("%s failed: 0x%x\n", __func__, ret);
            return ret;
        }
        
        return (id | (((u64) 4) << 32));
    }
}
