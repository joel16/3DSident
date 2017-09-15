#ifndef STORAGE_H
#define STORAGE_H

#include <3ds.h>

u64 getFreeStorage(FS_SystemMediaType mediaType);
u64 getTotalStorage(FS_SystemMediaType mediaType);
u64 getUsedStorage(FS_SystemMediaType mediaType);

#endif