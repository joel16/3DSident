#pragma once

#include <3ds.h>

namespace FS {
    Result OpenArchive(FS_Archive *archive, FS_ArchiveID archiveID);
    Result CloseArchive(FS_Archive archive);
    bool FileExists(FS_Archive archive, const char *path);
}
