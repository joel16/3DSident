#ifndef AC_H
#define AC_H

#include <3ds.h>

Result aciInit(void);
Result aciExit(void);
Result ACI_LoadWiFiSlot(u8 slot);
Result ACI_GetSSID(char * ssid);
Result ACI_GetPassphrase(char * passphrase);
Result ACI_GetSSIDLength(u8 * length);
Result ACI_GetSecurityMode(u8 * securityMode);

#endif
