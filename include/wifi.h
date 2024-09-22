#pragma once

namespace Wifi {
    bool GetSlot(u32 slot);
    const char *GetSSID(void);
    const char *GetPassphrase(void);
    const char *GetSecurityMode(void);
}
