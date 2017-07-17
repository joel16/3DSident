#include "actu.h"
#include "am.h"
#include "cfgs.h"
#include "system.h"
#include "utils.h"

const char * getModel(void)
{
    const char * models[] = 
	{
		"OLD 3DS - CTR",
		"OLD 3DS XL - SPR",
		"NEW 3DS - KTR",
		"OLD 2DS - FTR",
		"NEW 3DS XL - RED",
		"NEW 2DS XL - JAN",
		"Unknown"
    };

    u8 model = 0;
    CFGU_GetSystemModel(&model);

    if (model < 6)
        return models[model];
    else
        return models[6];
}

const char * getRegion(void)
{
    const char * regions[] = 
	{
        "JPN",
        "USA",
        "EUR",
        "AUS",
        "CHN",
        "KOR",
        "TWN",
        "Unknown"
    };

    u8 region = 0;
    CFGU_SecureInfoGetRegion(&region);

    if (region < 7)
        return regions[region];
    else
        return regions[7];
}

const char getFirmRegion(void)
{
	u8 canadaOrUsa = 0;
	CFGU_GetRegionCanadaUSA(&canadaOrUsa);
	
	if (strncmp(getRegion(), "JPN", 3) == 0)
		return 'J';
	else if (canadaOrUsa == 1)
		return 'U';
	else if ((strncmp(getRegion(), "EUR", 3) == 0) || (strncmp(getRegion(), "AUS", 3) == 0))
		return 'E';
	else if (strncmp(getRegion(), "CHN", 3) == 0)
		return 'C';
	else if (strncmp(getRegion(), "KOR", 3) == 0)
		return 'K';
	else if (strncmp(getRegion(), "TWN", 3) == 0)
		return 'T';
	else 
		return 'X';
}

const char * getLang(void)
{
    const char * languages[] = 
	{
        "Japanese",
        "English",
        "French",
        "German",
        "Italian",
        "Spanish",
        "Simplified Chinese",
        "Korean",
        "Dutch",
        "Portugese",
        "Russian",
        "Traditional Chinese"
    };

    u8 language;
    CFGU_GetSystemLanguage(&language);

    if (language < 11)
        return languages[language];
    else
        return languages[11];
}

char * getMacAddress(void)
{
    u8 * macByte = (u8*)0x1FF81060; 
    static char macAddress[18];

    //sprintf automatically zero-terminates the string
    snprintf(macAddress, 18, "%02X:%02X:%02X:%02X:%02X:%02X", *macByte, *(macByte + 1), *(macByte + 2), *(macByte + 3), *(macByte + 4), *(macByte + 5));

    return macAddress;
}

char * getScreenType(void)
{
	bool isNew3DS = 0;
	APT_CheckNew3DS(&isNew3DS);
	
	static char uScreenType[20];
	static char dScreenType[20];
	
	static char screenType[32];
	
	if (isNew3DS)
	{
		u8 screens = 0;
		
		if(R_SUCCEEDED(gspLcdInit()))
		{
			GSPLCD_GetVendors(&screens);
			gspLcdExit();
		}	
        
		switch ((screens >> 4) & 0xF)
		{
			case 1: // 0x01 = JDI => IPS
				sprintf(uScreenType, "Upper: IPS");
				break;
			case 0xC: // 0x0C = SHARP => TN
				sprintf(uScreenType, "Upper: TN");
				break;
			default:
				sprintf(uScreenType, "Upper: Unknown");
				break;
		}
		switch (screens & 0xF)
		{
			case 1: // 0x01 = JDI => IPS
				sprintf(dScreenType, " | Lower: IPS");
				break;
			case 0xC: // 0x0C = SHARP => TN
				sprintf(dScreenType, " | Lower: TN");
				break;
			default:
				sprintf(dScreenType, " | Lower: Unknown");
				break;
		}
		
		strcpy(screenType, uScreenType);
		strcat(screenType, dScreenType);
	}
	else
		sprintf(screenType, "Upper: TN | Lower: TN"); 
	
	return screenType;
}

u64 getLocalFriendCodeSeed(void)
{
	u64 seed = 0;
	
    PS_GetLocalFriendCodeSeed(&seed);
    
	return seed;
}

char * getSerialNum(void)
{
	static char str[32];
    char serial[0x10];
	
    cfgsSecureInfoGetSerialNo(serial);
    strcpy(str, serial);
    
	return str;
}

u32 getDeviceId(void) // Same as PS_GetDeviceId
{
    u32 tmp = 0;
    AM_GetDeviceId(&tmp);
    return tmp;
}

u64 getSoapId(void)
{
    u32 tmp = 0;
    AM_GetDeviceId(&tmp);
    return (tmp | (((u64) 4) << 32));
}

char * getDeviceCert(void)
{
    u8 const cert[0x180];
    amNetGetDeviceCert(cert);
    return base64Encode(cert, 0x180);
}

char * getNNIDInfo(u32 size, u32 blkId)
{
    char info[size];
	static char str[100];
	
    ACTU_GetAccountDataBlock(info, size, blkId);
	snprintf(str, size, "%s", info);
	
    return str;
}

char * getBrightness(u32 screen)
{
	u32 brightness = 0;
	
	if(R_SUCCEEDED(gspLcdInit()))
	{
		GSPLCD_GetBrightness(screen, &brightness);
		gspLcdExit();
	}	
	
	if (brightness == 0x10)
		return "1 (20%)";
	else if (brightness == 0x1C)
		return "2 (40%)";
	else if (brightness == 0x30)
		return "3 (60%)";
	else if (brightness == 0x52)
		return "4 (80%)";
	else if (brightness == 0x8E)
		return "5 (100%)";
	else
		return "n3DS only";
}

char * getCardSlotStatus(void)
{
	bool isInserted = false;
	FS_CardType cardType = 0;
	
	static char card[20];
	
	FSUSER_CardSlotIsInserted(&isInserted);
	
	if (isInserted)
	{
		FSUSER_GetCardType(&cardType);
		snprintf(card, 20, "inserted %s", cardType? "(TWL)" : "(CTR)");
		return card;
	}
	
	snprintf(card, 20, "not inserted");
	return card;
}