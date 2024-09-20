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
}
