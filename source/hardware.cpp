#include "hardware.h"
#include "utils.h"

#define REG_LCD_TOP_SCREEN    (u32)0x202200
#define REG_LCD_BOTTOM_SCREEN (u32)0x202A00

namespace Hardware {
    Result GetScreenType(gspLcdScreenType& top, gspLcdScreenType& bottom) {
        Result ret = 0;
        u8 vendors = 0;
        
        if (!Utils::IsNew3DS()) {
            top = GSPLCD_SCREEN_TN;
            bottom = GSPLCD_SCREEN_TN;
            return 0;
        }

        if (R_FAILED(ret = gspLcdInit())) {
            return ret;
        }

        if (R_FAILED(ret = GSPLCD_GetVendors(&vendors))) {
            return ret;
        }

        switch ((vendors >> 4) & 0xF) {
            case 0x01: // 0x01 = JDI => IPS
                top = GSPLCD_SCREEN_IPS;
                break;
                
            case 0x0C: // 0x0C = SHARP => TN
                top = GSPLCD_SCREEN_TN;
                break;
            default:
                top = GSPLCD_SCREEN_UNK;
                break;
        }

        switch (vendors & 0xF) {
            case 0x01: // 0x01 = JDI => IPS
                bottom = GSPLCD_SCREEN_IPS;
                break;
                
            case 0x0C: // 0x0C = SHARP => TN
                bottom = GSPLCD_SCREEN_TN;
                break;
                
            default:
                bottom = GSPLCD_SCREEN_UNK;
                break;
        }
        
        gspLcdExit();
        return 0;
    }

    bool GetAudioJackStatus(void) {
        Result ret = 0;
        bool status = false;

        if (R_FAILED(ret = DSP_GetHeadphoneStatus(std::addressof(status)))) {
            return false;
        }

        return status;
    }

    bool GetCardSlotStatus(void) {
        Result ret = 0;
        bool status = false;

        if (R_FAILED(ret = FSUSER_CardSlotIsInserted(std::addressof(status)))) {
            return false;
        }

        return status;
    }

    FS_CardType GetCardType(void) {
        Result ret = 0;
        FS_CardType type;

        if (R_FAILED(ret = FSUSER_GetCardType(std::addressof(type)))) {
            return CARD_CTR;
        }

        return type;
    }

    bool IsSdInserted(void) {
        Result ret = 0;
        bool detected = false;

        if (R_FAILED(ret = FSUSER_IsSdmcDetected(std::addressof(detected)))) {
            return false;
        }

        return detected;
    }

    const char *GetSoundOutputMode(void) {
        u8 data;
        const char *mode[] =  {
            "Mono",
            "Stereo",
            "Surround"
        };
        
        if (R_FAILED(CFGU_GetConfigInfoBlk2(sizeof(data), 0x00070001, std::addressof(data)))) {
            return "unknown";
        }

        return mode[data];
    }

    u32 GetBrightness(u32 screen) {
        Result ret = 0;
        u32 brightness = 0;
        u32 addr = (screen == GSPLCD_SCREEN_TOP? REG_LCD_TOP_SCREEN : REG_LCD_BOTTOM_SCREEN) + 0x40;

        if (R_FAILED(ret = gspInit())) {
            return ret;
        }

        if (R_FAILED(ret = GSPGPU_ReadHWRegs(addr, std::addressof(brightness), 4))) {
            gspExit();
            return ret;
        }

        gspExit();
        return brightness;
    }

    const char *GetAutoBrightnessStatus(void) {
        u8 data[0x8];
        
        if (R_FAILED(CFGU_GetConfigInfoBlk2(sizeof(data), 0x00050009, data))) {
            return "unknown";
        }
        
        return data[0x4] & 0xFF? "enabled" : "disabled";
    }
}
