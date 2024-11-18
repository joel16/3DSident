#include <3ds.h>

namespace FS {
    Result OpenArchive(FS_Archive *archive, FS_ArchiveID archiveID) {
        Result ret = 0;
        
        if (R_FAILED(ret = FSUSER_OpenArchive(archive, archiveID, fsMakePath(PATH_EMPTY, "")))) {
            return ret;
        }

        return 0;
    }

    Result CloseArchive(FS_Archive archive) {
        Result ret = 0;
        
        if (R_FAILED(ret = FSUSER_CloseArchive(archive))) {
            return ret;
        }

        return 0;
    }

    bool FileExists(FS_Archive archive, const char *path) {
        Handle handle;
        
        if (R_FAILED(FSUSER_OpenFile(&handle, archive, fsMakePath(PATH_ASCII, path), FS_OPEN_READ, 0))) {
            return false;
        }
        
        if (R_FAILED(FSFILE_Close(handle))) {
            return false;
        }
        
        return true;
    }
}
