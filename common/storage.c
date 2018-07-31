#include "storage.h"

u64 Storage_GetFreeStorage(FS_SystemMediaType mediaType)
{
	FS_ArchiveResource resource = {0};
	
	if (R_SUCCEEDED(FSUSER_GetArchiveResource(&resource, mediaType)))
		return (((u64) resource.freeClusters * (u64) resource.clusterSize));
	
	return 0;
}

u64 Storage_GetTotalStorage(FS_SystemMediaType mediaType)
{
	FS_ArchiveResource resource = {0};
	
	if (R_SUCCEEDED(FSUSER_GetArchiveResource(&resource, mediaType)))
		return (((u64) resource.totalClusters * (u64) resource.clusterSize));
	
	return 0;
}

u64 Storage_GetUsedStorage(FS_SystemMediaType mediaType)
{
	FS_ArchiveResource resource = {0};
	
	if (R_SUCCEEDED(FSUSER_GetArchiveResource(&resource, mediaType)))
		return ((((u64) resource.totalClusters * (u64) resource.clusterSize)) - (((u64) resource.freeClusters * (u64) resource.clusterSize)));
	
	return 0;
}
