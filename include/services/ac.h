#ifndef AC_H
#define AC_H

#include <3ds.h>

Result acGetServiceHandle(void);
Result acCloseServiceHandle(void);
Result ACU_GetConnectingSecurityMode(u32 * out);
Result ACU_GetConnectingSsid(u32 * out);
Result ACU_GetConnectingSsidLength(u32 * out);

#endif