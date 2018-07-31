#ifndef _3DSIDENT_KERNEL_H_
#define _3DSIDENT_KERNEL_H_

#include <3ds.h>

char *Kernel_GetInitalVersion(void);
char *Kernel_GetVersion(int version);
char *Kernel_GetSDMCCID(void);
char *Kernel_GetNANDCID(void);
u32 Kernel_GetDeviceId(void);

#endif