#include "am.h"
#include "misc.h"
#include "utils.h"

u32 titleCount(FS_MediaType mediaType)
{
	u32 count = 0;
	
	if (R_SUCCEEDED(AM_GetTitleCount(mediaType, &count)))
		return count;

    return 0;
}

char * getDeviceCert(void)
{
	u8 const cert[0x180];
	
	if (R_SUCCEEDED(amNetGetDeviceCert(cert)))
		return base64Encode(cert, 0x180);
	
	return NULL;
}