#include <memory>

#include "log.h"
#include "service.h"

namespace NNID {
    static const u8 defaultSlot = 0xFE;

    u32 GetPersistentId(void) {
        Result ret = 0;
        u32 persistentId;

        if (R_FAILED(ret = ACTU::GetAccountInfo(defaultSlot, sizeof(u32), 0x5, std::addressof(persistentId)))) {
            Log::Error("%s failed: 0x%x\n", __func__, ret);
            return ret;
        }

        return persistentId;
    }

    u64 GetTransferableIdBase(void) {
        Result ret = 0;
        u64 transferableIdBase;

        if (R_FAILED(ret = ACTU::GetAccountInfo(defaultSlot, sizeof(u64), 0x6, std::addressof(transferableIdBase)))) {
            Log::Error("%s failed: 0x%x\n", __func__, ret);
            return ret;
        }

        return transferableIdBase;
    }
    
    const char *GetAccountId(void) {
        Result ret = 0;
        static char accountId[0x11];

        if (R_FAILED(ret = ACTU::GetAccountInfo(defaultSlot, sizeof(accountId), 0x8, accountId))) {
            Log::Error("%s failed: 0x%x\n", __func__, ret);
            return "unknown";
        }

        return accountId;
    }

    const char *GetCountryName(void) {
        Result ret = 0;
        static char countryName[0x3];

        if (R_FAILED(ret = ACTU::GetAccountInfo(defaultSlot, sizeof(countryName), 0xB, countryName))) {
            Log::Error("%s failed: 0x%x\n", __func__, ret);
            return "unknown";
        }

        return countryName;
    }

    u32 GetPrincipalId(void) {
        Result ret = 0;
        u32 principalId;

        if (R_FAILED(ret = ACTU::GetAccountInfo(defaultSlot, sizeof(u32), 0xC, std::addressof(principalId)))) {
            Log::Error("%s failed: 0x%x\n", __func__, ret);
            return ret;
        }

        return principalId;
    }

    const char *IsServerAccountDeleted(void) {
        Result ret = 0;
        u8 accountDeleted;

        if (R_FAILED(ret = ACTU::GetAccountInfo(defaultSlot, sizeof(u8), 0x24, std::addressof(accountDeleted)))) {
            Log::Error("%s failed: 0x%x\n", __func__, ret);
            return "unknown";
        }

        return accountDeleted? "deleted" : "active";
    }
}
