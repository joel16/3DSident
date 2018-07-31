#ifndef _3DSIDENT_MISC_H_
#define _3DSIDENT_MISC_H_

#include <3ds.h>

u32 Misc_TitleCount(FS_MediaType mediaType);
u32 Misc_TicketCount(void);
char *Misc_GetDeviceCert(void);

#endif