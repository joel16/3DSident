#ifndef KERNEL_H
#define KERNEL_H

#include <3ds.h>

char * getVersion(int version);
char * getSdmcCid(void);
char * getNandCid(void);
u32 getDeviceId(void);

#endif