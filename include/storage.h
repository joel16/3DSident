#pragma once

#include <3ds.h>

namespace Storage {
    u64 GetFreeStorage(FS_SystemMediaType mediaType);
    u64 GetTotalStorage(FS_SystemMediaType mediaType);
    u64 GetUsedStorage(FS_SystemMediaType mediaType);
}
