#include <3ds.h>
#include <cstring>
#include <memory>

#include "fs.h"
#include "log.h"
#include "utils.h"

namespace Misc {
    u32 GetTitleCount(FS_MediaType mediaType) {
        Result ret = 0;
        u32 count = 0;

        if (R_FAILED(ret = AM_GetTitleCount(mediaType, std::addressof(count)))) {
#ifdef BUILD_DEBUG
            Log::Error("%s failed: 0x%x\n", __func__, ret);
#endif
            return ret;
        }

        return count;
    }

    u32 GetTicketCount(void) {
        Result ret = 0;
        u32 count = 0;

        if (R_FAILED(ret = AM_GetTicketCount(std::addressof(count)))) {
#ifdef BUILD_DEBUG
            Log::Error("%s failed: 0x%x\n", __func__, ret);
#endif
            return ret;
        }

        return count;
    }

    const char *GetManufacturingDate(void) {
        Result ret = 0;
        
        FS_Archive archive;
        if (R_FAILED(ret = FS::OpenArchive(std::addressof(archive), ARCHIVE_NAND_TWL_FS))) {
#ifdef BUILD_DEBUG
            Log::Error("%s(FS::OpenArchive) failed: 0x%x\n", __func__, ret);
#endif
            return "unknown";
        }
        
        Handle handle;
        if (R_FAILED(ret = FSUSER_OpenFileDirectly(&handle, ARCHIVE_NAND_TWL_FS, fsMakePath(PATH_EMPTY, ""), fsMakePath(PATH_ASCII, "/sys/log/inspect.log"), FS_OPEN_READ, 0))) {
#ifdef BUILD_DEBUG
            Log::Error("%s(FSUSER_OpenFileDirectly) failed: 0x%x\n", __func__, ret);
#endif
            FS::CloseArchive(archive);
            return "unknown";
        }

        u64 size = 0;
        if (R_FAILED(ret = FSFILE_GetSize(handle, std::addressof(size)))) {
#ifdef BUILD_DEBUG
            Log::Error("%s(FSFILE_GetSize) failed: 0x%x\n", __func__, ret);
#endif
            FSFILE_Close(handle);
            FS::CloseArchive(archive);
            return "unknown";
        }

        char *buf = new char[size + 1];
        u32 bytesRead = 0;
        
        if (R_FAILED(ret = FSFILE_Read(handle, std::addressof(bytesRead), 0, reinterpret_cast<u32 *>(buf), static_cast<u32>(size)))) {
#ifdef BUILD_DEBUG
            Log::Error("%s(FSFILE_Read) failed: 0x%x\n", __func__, ret);
#endif
            delete[] buf;
            FSFILE_Close(handle);
            FS::CloseArchive(archive);
            return "unknown";
        }
        
        buf[size] = '\0';
        
        static char date[11];
        std::string d = Utils::GetSubstring(buf, "CommentUpdated=", "\n");
        std::snprintf(date, sizeof(date), "%s", d.c_str());

        if (R_FAILED(ret = FSFILE_Close(handle))) {
#ifdef BUILD_DEBUG
            Log::Error("%s(FSFILE_Close) failed: 0x%x\n", __func__, ret);
#endif
            delete[] buf;
            FS::CloseArchive(archive);
            return "unknown";
        }

        delete[] buf;
        FS::CloseArchive(archive);
        return date;
    }
}
