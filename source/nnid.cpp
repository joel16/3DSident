#include <memory>

#include "service.h"

namespace NNID {
    u32 GetPersistentId(void) {
        Result ret = 0;
        u32 persistentId;

        if (R_FAILED(ret = ACTU::GetAccountDataBlock(0xFE, 0x5, sizeof(u32), std::addressof(persistentId)))) {
            return ret;
        }

        return persistentId;
    }

    u64 GetTransferableIdBase(void) {
        Result ret = 0;
        u64 transferableIdBase;

        if (R_FAILED(ret = ACTU::GetAccountDataBlock(0xFE, 0x6, sizeof(u64), std::addressof(transferableIdBase)))) {
            return ret;
        }

        return transferableIdBase;
    }

    // TODO: Fix/research
    const char *GetAccountId(void) {
        Result ret = 0;
        static char accountId[0x11];

        if (R_FAILED(ret = ACTU::GetAccountDataBlock(0xFE, 0x8, sizeof(accountId), accountId))) {
            return "unknown";
        }

        return accountId;
    }
    
    // TODO: Fix/research
    const char *GetCountryName(void) {
        Result ret = 0;
        static char countryName[0x3];

        if (R_FAILED(ret = ACTU::GetAccountDataBlock(0xFE, 0xB, sizeof(countryName), countryName))) {
            return "unknown";
        }

        return countryName;
    }

    u32 GetPrincipalId(void) {
        Result ret = 0;
        u32 principalId;

        if (R_FAILED(ret = ACTU::GetAccountDataBlock(0xFE, 0xC, sizeof(u32), std::addressof(principalId)))) {
            return ret;
        }

        return principalId;
    }
    
    // TODO: Fix/research
    const char *GetNfsPassword(void) {
        Result ret = 0;
        static char nfsPassword[0x11];

        if (R_FAILED(ret = ACTU::GetAccountDataBlock(0xFE, 0x1C, sizeof(nfsPassword), nfsPassword))) {
            return "unknown";
        }

        return nfsPassword;
    }
}
