#ifndef AM_H
#define AM_H

#include <3ds.h>

Result amGetServiceHandle(void);
Result amCloseServiceHandle(void);
Result amNetGetDeviceCert(u8 const * buffer);

#endif
