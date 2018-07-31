#ifndef _3DSIDENT_FS_H_
#define _3DSIDENT_FS_H_

#include <3ds.h>

FS_Archive archive;

Result FS_OpenArchive(FS_Archive * archive, FS_ArchiveID id);
Result FS_CloseArchive(FS_Archive archive);
Result FS_MakeDir(FS_Archive archive, const char * path);
Result FS_RecursiveMakeDir(FS_Archive archive, const char * dir);
bool FS_FileExists(FS_Archive archive, const char * path);
bool FS_DirExists(FS_Archive archive, const char * path);

#endif