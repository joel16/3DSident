#include "ac.h"
#include "wifi.h"

char * getSecurityMode()
{
	u32 securityMode = 0;
    
	if (R_FAILED(ACU_GetConnectingSecurityMode(&securityMode)))
		securityMode = 8;
    
	static char * securityString[] = 
	{
		"Open authentication",
		"WEP 40-bit",
		"WEP 104-bit",
		"WEP 128-bit",
		"WPA TKIP",
		"WPA2 TKIP",
		"WPA AES",
		"WPA2 AES",
		"Unknown"
	};
    
	return securityString[securityMode];
}