#include <3ds.h>
#include <cstdio>
#include <memory>

#include "fs.h"
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

    const char *GetNandLocalFriendCodeSeed(void) {
        Result ret = 0;
        Handle handle;
        u32 bytesread = 0;
        FS_Archive nandArchive;
        char buf[7];
        static char out[11];

        FS::OpenArchive(std::addressof(nandArchive), ARCHIVE_NAND_CTR_FS);
        
        if (FS::FileExists(nandArchive, "/rw/sys/LocalFriendCodeSeed_B")) {
            if (R_FAILED(ret = FSUSER_OpenFile(std::addressof(handle), nandArchive, fsMakePath(PATH_ASCII, "/rw/sys/LocalFriendCodeSeed_B"), FS_OPEN_READ, 0))) {
                FS::CloseArchive(nandArchive);
                return "";
            }
        }
        else if (FS::FileExists(nandArchive, "/rw/sys/LocalFriendCodeSeed_A")) {
            if (R_FAILED(ret = FSUSER_OpenFile(std::addressof(handle), nandArchive, fsMakePath(PATH_ASCII, "/rw/sys/LocalFriendCodeSeed_A"), FS_OPEN_READ, 0))) {
                FS::CloseArchive(nandArchive);
                return "";
            }
        }
        else {
            FS::CloseArchive(nandArchive);
            return "LocalFriendCodeSeed not found";
        }
        
        if (R_FAILED(ret = FSFILE_Read(handle, std::addressof(bytesread), 0x108, reinterpret_cast<u32 *>(buf), 6))) {
            Log::Error("%s(FSFILE_Read) failed: 0x%x\n", __func__, ret);
            FS::CloseArchive(nandArchive);
            return "unknown";
        }
        
        if (R_FAILED(ret = FSFILE_Close(handle))) {
            Log::Error("%s(FSFILE_Close) failed: 0x%x\n", __func__, ret);
            FS::CloseArchive(nandArchive);
            return "unknown";
        }

        FS::CloseArchive(nandArchive);
        buf[6] = '\0';
        snprintf(out, 11, "%02X%02X%02X%02X%02X", buf[4], buf[3], buf[2], buf[1], buf[0]);
        return out;
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
        int oddSum = 0, evenSum = 0, index = 1;
        
        for (int i = 0; serialNumber[i] != '\0'; i++) {
            if (isdigit(serialNumber[i])) {
                int digit = serialNumber[i] - '0';
                
                if (index % 2 == 0) {
                    evenSum += digit;
                }
                else {
                    oddSum += digit;
                }

                index++;
            }
        }
        
        int checkDigit = ((3 * evenSum) + oddSum) % 10;
        return checkDigit == 0 ? 0 : 10 - checkDigit;
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
