#ifndef MISC_H
#define MISC_H

#include <3ds.h>

u32 titleCount(FS_MediaType mediaType);
bool detectSD();
u64 getFreeStorage(FS_SystemMediaType mediaType);
u64 getTotalStorage(FS_SystemMediaType mediaType);
u64 getUsedStorage(FS_SystemMediaType mediaType);

#endif