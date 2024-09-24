#include <memory>

#include "service.h"

namespace NNID {
    u32 GetPersistentId(void) {
        Result ret = 0;
        u32 persistentId;

        if (R_FAILED(ret = ACTU::GetAccountDataBlock(0xFE, 0x5, 0x4, std::addressof(persistentId)))) {
            return ret;
        }

        return persistentId;
    }

    u64 GetTransferableIdBase(void) {
        Result ret = 0;
        u64 transferableIdBase;

        if (R_FAILED(ret = ACTU::GetAccountDataBlock(0xFE, 0x6, 0x8, std::addressof(transferableIdBase)))) {
            return ret;
        }

        return transferableIdBase;
    }

    const char *GetAccountId(void) {
        Result ret = 0;
        static char accountId[0x11];

        if (R_FAILED(ret = ACTU::GetAccountDataBlock(0xFE, 0x8, 0x11, accountId))) {
            return "unknown";
        }

        return accountId;
    }

    const char *GetCountryName(void) {
        Result ret = 0;
        static char countryName[0x3];

        if (R_FAILED(ret = ACTU::GetAccountDataBlock(0xFE, 0xB, 0x3, countryName))) {
            return "unknown";
        }

        return countryName;
    }

    u32 GetPrincipalId(void) {
        Result ret = 0;
        u32 principalId;

        if (R_FAILED(ret = ACTU::GetAccountDataBlock(0xFE, 0xC, 0x4, std::addressof(principalId)))) {
            return ret;
        }

        return principalId;
    }

    const char *GetNfsPassword(void) {
        Result ret = 0;
        static char nfsPassword[0x11];

        if (R_FAILED(ret = ACTU::GetAccountDataBlock(0xFE, 0x1C, 0x11, nfsPassword))) {
            return "unknown";
        }

        return nfsPassword;
    }
}
