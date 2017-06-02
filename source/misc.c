#include "misc.h"

u32 titleCount(FS_MediaType mediaType)
{
	u32 count = 0;
	
	AM_GetTitleCount(mediaType, &count);

    return count;
}

bool detectSD()
{
    bool isSD;
    FSUSER_IsSdmcDetected(&isSD);
    return isSD;
}