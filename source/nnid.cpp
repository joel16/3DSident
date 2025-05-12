#include <memory>

#include "log.h"
#include "service.h"

namespace NNID {
    u32 GetPersistentId(void) {
        Result ret = 0;
        u32 persistentId;

        if (R_FAILED(ret = ACT_GetAccountInfo(std::addressof(persistentId), sizeof(u32), ACT_DEFAULT_ACCOUNT, INFO_TYPE_PERSISTENT_ID))) {
            Log::Error("%s failed: 0x%x\n", __func__, ret);
            return ret;
        }

        return persistentId;
    }

    u64 GetTransferableIdBase(void) {
        Result ret = 0;
        u64 transferableIdBase;

        if (R_FAILED(ret = ACT_GetAccountInfo(std::addressof(transferableIdBase), sizeof(u64), ACT_DEFAULT_ACCOUNT, INFO_TYPE_COMMON_TRANSFERABLE_ID_BASE))) {
            Log::Error("%s failed: 0x%x\n", __func__, ret);
            return ret;
        }

        return transferableIdBase;
    }
    
    const char *GetAccountId(void) {
        Result ret = 0;
        static char accountId[0x11];

        if (R_FAILED(ret = ACT_GetAccountInfo(accountId, sizeof(accountId), ACT_DEFAULT_ACCOUNT, INFO_TYPE_ACCOUNT_ID))) {
            Log::Error("%s failed: 0x%x\n", __func__, ret);
            return "unknown";
        }

        return accountId;
    }

    const char *GetCountryName(void) {
        Result ret = 0;
        static char countryName[0x3];

        if (R_FAILED(ret = ACT_GetAccountInfo(countryName, sizeof(countryName), ACT_DEFAULT_ACCOUNT, INFO_TYPE_COUNTRY_NAME))) {
            Log::Error("%s failed: 0x%x\n", __func__, ret);
            return "unknown";
        }

        return countryName;
    }

    u32 GetPrincipalId(void) {
        Result ret = 0;
        u32 principalId;

        if (R_FAILED(ret = ACT_GetAccountInfo(std::addressof(principalId), sizeof(u32), ACT_DEFAULT_ACCOUNT, INFO_TYPE_PRINCIPAL_ID))) {
            Log::Error("%s failed: 0x%x\n", __func__, ret);
            return ret;
        }

        return principalId;
    }

    const char *IsServerAccountDeleted(void) {
        Result ret = 0;
        u8 accountDeleted;

        if (R_FAILED(ret = ACT_GetAccountInfo(std::addressof(accountDeleted), sizeof(u8), ACT_DEFAULT_ACCOUNT, INFO_TYPE_IS_SERVER_ACCOUNT_DELETED))) {
            Log::Error("%s failed: 0x%x\n", __func__, ret);
            return "unknown";
        }

        return accountDeleted? "deleted" : "active";
    }
}
