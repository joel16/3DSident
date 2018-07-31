#ifndef _3DSIDENT_SYSTEM_H_
#define _3DSIDENT_SYSTEM_H_

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

enum 
{
	DATETIME                = 0x1FF81000,
	RUNNING_HW              = 0x1FF81004,
	MCU_HW_INFO             = 0x1FF81005,
	DATETIME_0              = 0x1FF81020,
	DATETIME_1              = 0x1FF81040,
	WIFI_MACADDR            = 0x1FF81060,
	WIFI_LINKLEVEL          = 0x1FF81066,
	_3D_SLIDERSTATE         = 0x1FF81080,
	_3D_LEDSTATE            = 0x1FF81084,
	BATTERY_LEDSTATE        = 0x1FF81085,
	MENUTID             	= 0x1FF810A0,
	ACTIVEMENUTID           = 0x1FF810A8,
	HEADSET_CONNECTED       = 0x1FF810C0
};

const char *System_GetModel(void);
const char *System_GetRegion(void);
const char System_GetFirmRegion(void);
bool System_IsCoppacsSupported();
const char *System_GetLang(void);
char *System_GetMacAddress(void);
const char *System_GetRunningHW(void);
char *System_IsDebugUnit(void);
char *System_GetScreenType(void);
u64 System_GetLocalFriendCodeSeed(void);
u8 *System_GetSerialNumber(void);
u64 System_GetSoapId(void);


#endif