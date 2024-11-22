#include <cstdarg>
#include <string>

#include "fs.h"
#include "log.h"

namespace Log {
    static FS_Archive sdmcArchive;
    static Handle handle = 0;
    static u64 offset = 0;

    Result Open(void) {
        Result ret = 0;
        const char *path = "/3ds/3dsident.log";
        
        FS::OpenArchive(std::addressof(sdmcArchive), ARCHIVE_SDMC);

        // Delete existing logs on start up.
        if (FS::FileExists(sdmcArchive, path)) {
            FSUSER_DeleteFile(sdmcArchive, fsMakePath(PATH_ASCII, path));
        }

        if (!FS::FileExists(sdmcArchive, path)) {
            if (R_FAILED(ret = FSUSER_CreateFile(sdmcArchive, fsMakePath(PATH_ASCII, path), 0, 0))) {
                return ret;
            }
        }
        
        if (R_FAILED(ret = FSUSER_OpenFile(&handle, sdmcArchive, fsMakePath(PATH_ASCII, path), FS_OPEN_WRITE, 0))) {
            return ret;
        }
            
        return 0;
    }
    
    Result Close(void) {
        Result ret = 0;
        
        if (R_FAILED(ret = FSFILE_Close(handle))) {
            return ret;
        }

        FS::CloseArchive(sdmcArchive);
        return 0;
    }

    void Error(const char *data, ...) {
        // File handle was not open for writing
        if (!handle) {
            return;
        }

        char buf[256];
        va_list args;
        va_start(args, data);
        std::vsnprintf(buf, sizeof(buf), data, args);
        va_end(args);
        
        std::string error_string = "[ERROR] ";
        error_string.append(buf);
        
        std::printf("%s", error_string.c_str());

        u32 bytes_written = 0;
        if (R_FAILED(FSFILE_Write(handle, &bytes_written, offset, error_string.data(), error_string.length(), FS_WRITE_FLUSH))) {
            return;
        }
            
        offset += bytes_written;
    }
}
