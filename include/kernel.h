#ifndef KERNEL_H
#define KERNEL_H

#include <3ds.h>

enum 
{
	KERNEL_VERSIONREVISION  = 0x1FF80001,
	KERNEL_VERSIONMINOR     = 0x1FF80002,
	KERNEL_VERSIONMAJOR     = 0x1FF80003,
	UPDATEFLAG              = 0x1FF80004,
	NSTID                   = 0x1FF80008,
	SYSCOREVER              = 0x1FF80010,
	ENVINFO                 = 0x1FF80014,
	UNITINFO                = 0x1FF80014,
	PREV_FIRM               = 0x1FF80016,
	KERNEL_CTRSDKVERSION    = 0x1FF80018,
	APPMEMTYPE              = 0x1FF80030,
	APPMEMALLOC             = 0x1FF80040,
	SYSMEMALLOC             = 0x1FF80044,
	BASEMEMALLOC            = 0x1FF80048,
	FIRM_VERSIONREVISION    = 0x1FF80061,
	FIRM_VERSIONMINOR       = 0x1FF80062,
	FIRM_VERSIONMAJOR       = 0x1FF80063,
	FIRM_SYSCOREVER         = 0x1FF80064,
	FIRM_CTRSDKVERSION      = 0x1FF80068,
};

char * getVersion(int version);
char * getSdmcCid(void);
char * getNandCid(void);
u32 getDeviceId(void);

#endif