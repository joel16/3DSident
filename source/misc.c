#include "misc.h"

u32 titleCount(FS_MediaType mediaType)
{
	u32 count = 0;
	
	AM_GetTitleCount(mediaType, &count);

    return count;
}

bool detectSD(void)
{
    bool isSD;
    FSUSER_IsSdmcDetected(&isSD);
    return isSD;
}

u64 getFreeStorage(FS_SystemMediaType mediaType)
{
	FS_ArchiveResource	resource = {0};
	
	FSUSER_GetArchiveResource(&resource, mediaType);
	
	return (((u64) resource.freeClusters * (u64) resource.clusterSize));
}

u64 getTotalStorage(FS_SystemMediaType mediaType)
{
	FS_ArchiveResource	resource = {0};
	
	FSUSER_GetArchiveResource(&resource, mediaType);
	
	return (((u64) resource.totalClusters * (u64) resource.clusterSize));
}

u64 getUsedStorage(FS_SystemMediaType mediaType)
{
	FS_ArchiveResource	resource = {0};
	
	FSUSER_GetArchiveResource(&resource, mediaType);
	
	return ((((u64) resource.totalClusters * (u64) resource.clusterSize)) - (((u64) resource.freeClusters * (u64) resource.clusterSize)));
}