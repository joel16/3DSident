#include <3ds.h>
#include <memory>

#include "log.h"

namespace Misc {
    u32 GetTitleCount(FS_MediaType mediaType) {
        Result ret = 0;
        u32 count = 0;

        if (R_FAILED(ret = AM_GetTitleCount(mediaType, std::addressof(count)))) {
            Log::Error("%s failed: 0x%x\n", __func__, ret);
            return ret;
        }

        return count;
    }

    u32 GetTicketCount(void) {
        Result ret = 0;
        u32 count = 0;

        if (R_FAILED(ret = AM_GetTicketCount(std::addressof(count)))) {
            Log::Error("%s failed: 0x%x\n", __func__, ret);
            return ret;
        }

        return count;
    }
}
