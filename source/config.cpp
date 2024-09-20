#include <3ds.h>
#include <cstdio>

#include "utils.h"

namespace Config {

    struct UsernameBlock {
        u16 username[10];
        u32 zero;
        u32 ngWord;
    };
    
    struct EulaVersionBlock {
        u8 minor;
        u8 major;
        u8 padding[2];
    };
    
    struct BirthdayBlock {
        u8 month;
        u8 day;
    };

    struct ParentalControlBlock {
        u8 unk[13];
        u8 pin[4];
        u16 secretAnswer[32];
    };
    
    struct BacklightControlBlock {
        u8 powerSavingEnabled;
        u8 brightnessLevel;
    };
    
    const char *GetUsername(void) {
        UsernameBlock usernameBlock;
        
        if (R_FAILED(CFGU_GetConfigInfoBlk2(sizeof(UsernameBlock), 0x000A0000, std::addressof(usernameBlock)))) {
            return "unknown";
        }

        static char username[10];
        Utils::UTF16ToUTF8(reinterpret_cast<u8 *>(username), usernameBlock.username, 10);
        return username;
    }

    const char *GetBirthday(void) {
        BirthdayBlock birthdayBlock;

        if (R_FAILED(CFGU_GetConfigInfoBlk2(sizeof(BirthdayBlock), 0x000A0001, std::addressof(birthdayBlock)))) {
            return "unknown";
        }
        
        const char *months[] = {
            "January", "February", "March", "April", "May", "June",
            "July", "August", "September", "October", "November", "December"
        };

        static char date[15];
        std::snprintf(date, 15, "%s %02d", months[birthdayBlock.month - 1], birthdayBlock.day);
        return date;
    }
    
    const char *GetEulaVersion(void) {
        EulaVersionBlock eulaVersionBlock;

        if (R_FAILED(CFGU_GetConfigInfoBlk2(sizeof(EulaVersionBlock), 0x000D0000, std::addressof(eulaVersionBlock)))) {
            return "unknown";
        }

        static char version[6];
        std::snprintf(version, 6, "%1X.%02X", eulaVersionBlock.major, eulaVersionBlock.minor);
        return version;
    }

    const char *GetParentalPin(void) {
        ParentalControlBlock parentalControlBlock;
        
        if (R_FAILED(CFG_GetConfigInfoBlk8(sizeof(ParentalControlBlock), 0x00100001, std::addressof(parentalControlBlock)))) {
            return "unknown";
        }
        
        static char pin[5];
        std::snprintf(pin, 5, "%u%u%u%u", (parentalControlBlock.pin[0] - 0x30), (parentalControlBlock.pin[1] - 0x30),
            (parentalControlBlock.pin[2] - 0x30), (parentalControlBlock.pin[3] - 0x30));
        return pin;
    }
    
    const char *GetParentalEmail(void) {
        u8 data[0x200];

        if (R_FAILED(CFGU_GetConfigInfoBlk2(sizeof(data), 0x000C0002, data))) {
            return "unknown";
        }

        static char email[0x200];
        std::snprintf(email, 0x200, "%s", (data + 1));
        return email;
    }

    const char *GetParentalSecretAnswer(void) {
        u8 data[0x94]; // block 0x00100001 is of size 0x94

        if (R_FAILED(CFG_GetConfigInfoBlk8(0x94, 0x00100001, data))) {
            return "unknown";
        }

        static char out[0x21];
        Utils::UTF16ToUTF8(reinterpret_cast<u8 *>(out), reinterpret_cast<u16 *>(data + 16), 0x21);
        return out + 1;
    }

    const char *GetPowersaveStatus(void) {
        BacklightControlBlock backlightControlBlock;
        
        if (R_FAILED(CFG_GetConfigInfoBlk8(sizeof(BacklightControlBlock), 0x00050001, std::addressof(backlightControlBlock)))) {
            return "unknown";
        }

        return backlightControlBlock.powerSavingEnabled? "enabled" : "disabled";
    }
}
