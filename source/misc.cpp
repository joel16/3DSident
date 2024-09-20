#include <3ds.h>
#include <memory>

namespace Misc {
    u32 GetTitleCount(FS_MediaType mediaType) {
        Result ret = 0;
        u32 count = 0;

        if (R_FAILED(ret = AM_GetTitleCount(mediaType, std::addressof(count)))) {
            return ret;
        }

        return count;
    }

    u32 GetTicketCount(void) {
        Result ret = 0;
        u32 count = 0;

        if (R_FAILED(ret = AM_GetTicketCount(std::addressof(count)))) {
            return ret;
        }

        return count;
    }
}
