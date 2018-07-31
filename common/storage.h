#ifndef _3DSIDENT_STORAGE_H_
#define _3DSIDENT_STORAGE_H_

#include <3ds.h>

u64 Storage_GetFreeStorage(FS_SystemMediaType mediaType);
u64 Storage_GetTotalStorage(FS_SystemMediaType mediaType);
u64 Storage_GetUsedStorage(FS_SystemMediaType mediaType);

#endif