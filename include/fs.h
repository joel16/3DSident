#ifndef FS_H
#define FS_H

#include <3ds.h>

FS_Archive fsArchive;

Result openArchive(FS_Archive * archive, FS_ArchiveID id);
Result closeArchive(FS_Archive archive);
Result makeDir(FS_Archive archive, const char * path);
bool fileExists(FS_Archive archive, const char * path);
bool dirExists(FS_Archive archive, const char * path);

#endif