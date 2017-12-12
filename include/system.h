#ifndef SYSTEM_H
#define SYSTEM_H

#include <3ds.h>

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

const char * getModel(void);
const char * getRegion(void);
const char getFirmRegion(void);
const char * getLang(void);
char * getMacAddress(void);
char * getRunningHW(void);
char * isDebugUnit(void);
char * getScreenType(void);
u64 getLocalFriendCodeSeed(void);
u8 * getSerialNumber(void);
u64 getSoapId(void);
char * getSecurityMode();

#endif