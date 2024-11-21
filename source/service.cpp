#include <3ds.h>
#include <cstring>

#include "config.h"
#include "hardware.h"
#include "kernel.h"
#include "misc.h"
#include "nnid.h"
#include "service.h"
#include "storage.h"
#include "system.h"
#include "utils.h"
#include "wifi.h"

namespace ACI {
    Result GetSecurityMode(acSecurityMode *mode) {
        Result ret = 0;
        u32 *cmdbuf = getThreadCommandBuffer();
        
        cmdbuf[0] = IPC_MakeHeader(0x413,0,0); // 0x04130000
        
        if (R_FAILED(ret = svcSendSyncRequest(*acGetSessionHandle()))) {
            return ret;
        }
        
        *mode = static_cast<acSecurityMode>(cmdbuf[2]);
        return static_cast<Result>(cmdbuf[1]);
    }

    Result GetPassphrase(char *passphrase) {
        Result ret = 0;
        u32 *cmdbuf = getThreadCommandBuffer();
        
        cmdbuf[0] = IPC_MakeHeader(0x415,0,0); // 0x04150000
        
        u32* staticbufs = getThreadStaticBuffers();
        staticbufs[0] = IPC_Desc_StaticBuffer(64, 0); // Password length is 64
        staticbufs[1] = reinterpret_cast<u32>(passphrase);
        
        if (R_FAILED(ret = svcSendSyncRequest(*acGetSessionHandle()))) {
            return ret;
        }

        return static_cast<Result>(cmdbuf[1]);
    }
}

namespace ACTU {
    static Handle actHandle;
    static int actRefCount;
    
    Result Init(void) {
        Result ret = 0;
        
        if (AtomicPostIncrement(std::addressof(actRefCount))) {
            return 0;
        }
        
        ret = srvGetServiceHandle(std::addressof(actHandle), "act:u");
        
        if (R_FAILED(ret)) {
            ret = srvGetServiceHandle(std::addressof(actHandle), "act:a");
        }
        
        if (R_FAILED(ret)) {
            AtomicDecrement(std::addressof(actRefCount));
        }
        
        return ret;
    }

    void Exit(void) {
        if (AtomicDecrement(std::addressof(actRefCount))) {
            return;
        }
        
        svcCloseHandle(actHandle);
    }

    Result GetAccountDataBlock(u8 slot, u32 size, u32 blkId, void *out) {
        Result ret = 0;
        u32 *cmdbuf = getThreadCommandBuffer();
        
        cmdbuf[0] = IPC_MakeHeader(0x6,3,2); // 0x00600C2
        cmdbuf[1] = slot;
        cmdbuf[2] = size;
        cmdbuf[3] = blkId;
        cmdbuf[4] = IPC_Desc_Buffer(size,IPC_BUFFER_W);
        cmdbuf[5] = reinterpret_cast<u32>(out);
        
        if (R_FAILED(ret = svcSendSyncRequest(actHandle))) {
            return ret;
        }
        
        return static_cast<Result>(cmdbuf[1]);
    }
}

namespace MCUHWC {
    Result GetBatteryTemperature(u8 *temp) {
        Result ret = 0;
        u32 *cmdbuf = getThreadCommandBuffer();
        
        cmdbuf[0] = IPC_MakeHeader(0xE,2,0); // 0x000E0080
        
        if (R_FAILED(ret = svcSendSyncRequest(*mcuHwcGetSessionHandle()))) {
            return ret;
        }
        
        *temp = cmdbuf[2];
        return static_cast<Result>(cmdbuf[1]);
    }
}

namespace Service {
    void Init(void) {
        acInit();
        ACTU::Init();
        amInit();
    }

    void Exit(void) {
        amExit();
        ACTU::Exit();
        acExit();
    }

    KernelInfo GetKernelInfo(void) {
        KernelInfo info = { 0 };
        info.kernelVersion = Kernel::GetVersion(VERSION_INFO_KERNEL);
        info.firmVersion = Kernel::GetVersion(VERSION_INFO_FIRM);
        info.systemVersion = Kernel::GetVersion(VERSION_INFO_SYSTEM);
        info.initialVersion = Kernel::GetInitalVersion();
        info.sdmcCid = Kernel::GetSdmcCid();
        info.nandCid = Kernel::GetNandCid();
        info.deviceId = Kernel::GetDeviceId();
        return info;
    }

    SystemInfo GetSystemInfo(void) {
        SystemInfo info = { 0 };
        info.model = System::GetModel();
        info.hardware = System::GetRunningHW();
        info.region = System::GetRegion();
        info.language = System::GetLanguage();
        info.localFriendCodeSeed = System::GetLocalFriendCodeSeed();
        info.nandLocalFriendCodeSeed = System::GetNandLocalFriendCodeSeed();
        info.macAddress = System::GetMacAddress();
        info.serialNumber = System::GetSerialNumber();
        info.checkDigit =  System::GetCheckDigit(info.serialNumber);
        info.soapId = System::GetSoapId();
        return info;
    }
    
    NNIDInfo GetNNIDInfo(void) {
        NNIDInfo info = { 0 };
        info.persistentID = NNID::GetPersistentId();
        info.transferableIdBase = NNID::GetTransferableIdBase();
        info.accountId = NNID::GetAccountId();
        info.countryName = NNID::GetCountryName();
        info.principalID = NNID::GetPrincipalId();
        info.nfsPassword = NNID::GetNfsPassword();
        return info;
    }

    ConfigInfo GetConfigInfo(void) {
        ConfigInfo info = { 0 };
        info.username = Config::GetUsername();
        info.birthday = Config::GetBirthday();
        info.eulaVersion = Config::GetEulaVersion();
        info.parentalPin = Config::GetParentalPin();
        info.parentalEmail = Config::GetParentalEmail();
        info.parentalSecretAnswer = Config::GetParentalSecretAnswer();
        return info;
    }

    HardwareInfo GetHardwareInfo(void) {
        HardwareInfo info = { 0 };

        gspLcdScreenType top, bottom;
        Hardware::GetScreenType(top, bottom);

        if (top == GSPLCD_SCREEN_UNK) {
            info.screenUpper = "unknown";
        }
        else {
            info.screenUpper = (top == GSPLCD_SCREEN_TN)? "TN" : "IPS";
        }

        if (bottom == GSPLCD_SCREEN_UNK) {
            info.screenLower = "unknown";
        }
        else {
            info.screenLower = (bottom == GSPLCD_SCREEN_TN)? "TN" : "IPS";
        }

        info.soundOutputMode = Hardware::GetSoundOutputMode();
        return info;
    }

    MiscInfo GetMiscInfo(void) {
        MiscInfo info = { 0 };
        info.sdTitleCount = Misc::GetTitleCount(MEDIATYPE_SD);
        info.nandTitleCount = Misc::GetTitleCount(MEDIATYPE_NAND);
        info.ticketCount = Misc::GetTicketCount();
        return info;
    }

    WifiInfo GetWifiInfo(void) {
        WifiInfo info = { 0 };

        for (u32 i = 0; i < 3; i++) {
            if (R_SUCCEEDED(ACI_LoadNetworkSetting(i))) {
                info.slot[i] = true;
                std::strncpy(info.ssid[i], Wifi::GetSSID(), 32);
                std::strncpy(info.passphrase[i], Wifi::GetPassphrase(), 64);
                std::strncpy(info.securityMode[i], Wifi::GetSecurityMode(), 12);
            }
        }

        return info;
    }

    StorageInfo GetStorageInfo(void) {
        StorageInfo info = { 0 };
        
        for (int i = 0; i < 4; i++) {
            info.usedSize[i] = Storage::GetUsedStorage(static_cast<FS_SystemMediaType>(i));
            info.totalSize[i] = Storage::GetTotalStorage(static_cast<FS_SystemMediaType>(i));
            Utils::GetSizeString(info.freeSizeString[i], Storage::GetFreeStorage(static_cast<FS_SystemMediaType>(i)));
            Utils::GetSizeString(info.usedSizeString[i], Storage::GetUsedStorage(static_cast<FS_SystemMediaType>(i)));
            Utils::GetSizeString(info.totalSizeString[i], Storage::GetTotalStorage(static_cast<FS_SystemMediaType>(i)));
        }

        return info;
    }

    SystemStateInfo GetSystemStateInfo(void) {
        SystemStateInfo info = { 0 };

        if (R_FAILED(MCUHWC_ReadRegister(0x7F, std::addressof(info), sizeof(SystemStateInfo)))) {
            return info;
        }

        return info;
    }
}
