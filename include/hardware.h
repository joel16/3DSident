#pragma once

#include <3ds.h>

typedef enum {
    GSPLCD_SCREEN_TN,
    GSPLCD_SCREEN_IPS,
    GSPLCD_SCREEN_UNK
} gspLcdScreenType;

namespace Hardware {
    Result GetScreenType(gspLcdScreenType& top, gspLcdScreenType& bottom);
    bool GetAudioJackStatus(void);
    bool GetCardSlotStatus(void);
    FS_CardType GetCardType(void);
    bool IsSdInserted(void);
    const char *GetSoundOutputMode(void);
    u32 GetBrightness(u32 screen);
    const char *GetAutoBrightnessStatus(void);
}
