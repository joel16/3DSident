#include <3ds.h>
#include <memory>

#include "service.h"

namespace Wifi {
    const char *GetSSID(void) {
        Result ret = 0;
        static char ssid[32];
        
        if (R_FAILED(ret = ACI_GetNetworkWirelessEssidSecuritySsid(ssid))) {
            return "unknown";
        }
        
        return ssid;
    }

    const char *GetPassphrase(void) {
        Result ret = 0;
        static char passphrase[64];
        
        if (R_FAILED(ret = ACI::GetPassphrase(passphrase))) {
            return "unknown";
        }
        
        return passphrase;
    }

    const char *GetSecurityMode(void) {
        Result ret = 0;
        acSecurityMode mode = AC_OPEN;

        const char *securityMode[] = {
            "not secured",
            "WEP 40-bit",
            "WEP 104-bit",
            "WEP 128-bit",
            "WPA TKIP",
            "WPA2 TKIP",
            "WPA AES",
            "WPA2 AES"
        };

        if (R_FAILED(ret = ACI::GetSecurityMode(std::addressof(mode)))) {
            return "unknown";
        }

        return securityMode[mode];
    }
}
