#ifndef FS_H
#define FS_H

#include <3ds.h>

FS_Archive fsArchive;

void openArchive(FS_ArchiveID id);
void closeArchive(void);
Result makeDir(FS_Archive archive, const char * path);
bool fileExists(FS_Archive archive, const char * path);
bool dirExists(FS_Archive archive, const char * path);

#endif