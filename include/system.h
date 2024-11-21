#pragma once

namespace System {
    const char *GetModel(void);
    const char *GetRegion(void);
    const char *GetFirmRegion(void);
    bool IsCoppacsSupported(void);
    const char *GetLanguage(void);
    const char *GetMacAddress(void);
    const char *GetRunningHW(void);
    const char *IsDebugUnit(void);
    u64 GetLocalFriendCodeSeed(void);
    const char *GetNandLocalFriendCodeSeed(void);
    u8 *GetSerialNumber(void);
    int GetCheckDigit(const u8* serialNumber);
    u64 GetSoapId(void);
}
