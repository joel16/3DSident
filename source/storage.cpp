#include <3ds.h>
#include <memory>

namespace Storage {
    static FS_ArchiveResource GetResource(FS_SystemMediaType mediaType) {
        Result ret = 0;
        FS_ArchiveResource resource = {0};

        if (R_FAILED(ret = FSUSER_GetArchiveResource(std::addressof(resource), mediaType))) {
            return resource;
        }

        return resource;
    }

    u64 GetFreeStorage(FS_SystemMediaType mediaType) {
        FS_ArchiveResource resource = Storage::GetResource(mediaType);
        return ((static_cast<u64>(resource.freeClusters) * static_cast<u64>(resource.clusterSize)));
    }

    u64 GetTotalStorage(FS_SystemMediaType mediaType) {
        FS_ArchiveResource resource = Storage::GetResource(mediaType);
        return ((static_cast<u64>(resource.totalClusters) * static_cast<u64>(resource.clusterSize)));
    }

    u64 GetUsedStorage(FS_SystemMediaType mediaType) {
        return Storage::GetTotalStorage(mediaType) - Storage::GetFreeStorage(mediaType);
    }
}
