#include "storage.h"

u64 getFreeStorage(FS_SystemMediaType mediaType)
{
	FS_ArchiveResource	resource = {0};
	
	if (R_SUCCEEDED(FSUSER_GetArchiveResource(&resource, mediaType)))
		return (((u64) resource.freeClusters * (u64) resource.clusterSize));
	
	return 0;
}

u64 getTotalStorage(FS_SystemMediaType mediaType)
{
	FS_ArchiveResource	resource = {0};
	
	if (R_SUCCEEDED(FSUSER_GetArchiveResource(&resource, mediaType)))
		return (((u64) resource.totalClusters * (u64) resource.clusterSize));
	
	return 0;
}

u64 getUsedStorage(FS_SystemMediaType mediaType)
{
	FS_ArchiveResource	resource = {0};
	
	if (R_SUCCEEDED(FSUSER_GetArchiveResource(&resource, mediaType)))
		return ((((u64) resource.totalClusters * (u64) resource.clusterSize)) - (((u64) resource.freeClusters * (u64) resource.clusterSize)));
	
	return 0;
}