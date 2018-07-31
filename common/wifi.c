#include "ac.h"
#include "utils.h"
#include "wifi.h"

const char *WiFi_GetSecurityMode(void)
{
	u8 securityMode = 0;
    
	if (R_FAILED(ACI_GetSecurityMode(&securityMode)))
		securityMode = 8;
    
	const char *securityString[] = 
	{
		"Not secured",
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
