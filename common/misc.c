#include "am.h"
#include "misc.h"
#include "utils.h"

u32 Misc_TitleCount(FS_MediaType mediaType)
{
	u32 count = 0;
	
	if (R_SUCCEEDED(AM_GetTitleCount(mediaType, &count)))
		return count;

	return 0;
}

u32 Misc_TicketCount(void)
{
	u32 count = 0;
	
	if (R_SUCCEEDED(AM_GetTicketCount(&count)))
		return count;

	return 0;
}

char *Misc_GetDeviceCert(void)
{
	u8 const cert[0x180];
	
	if (R_SUCCEEDED(amNetGetDeviceCert(cert)))
		return Utils_Base64Encode(cert, 0x180);

	return NULL;
}
