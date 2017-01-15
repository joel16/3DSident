#include "actu.h"
#include "am.h"
#include "cfgs.h"
#include "frd.h"
#include "system.h"
#include "utils.h"

const char * getModel()
{
    const char *models[] = 
	{
        "O3DS",
        "O3DS XL",
        "N3DS",
        "2DS",
        "N3DS XL",
        "Unknown"
    };

    u8 model = 0;
    CFGU_GetSystemModel(&model);

    if (model < 5)
        return models[model];
    else
        return models[5];
}

const char * getRegion()
{
    const char *regions[] = 
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

const char * getLang()
{
    const char *languages[] = 
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

char * getMacAddress()
{
    u8* macByte = (u8*)0x1FF81060; 
    static char macAddress[18];

    //sprintf automatically zero-terminates the string
    sprintf(macAddress, "%02X:%02X:%02X:%02X:%02X:%02X", *macByte, *(macByte + 1), *(macByte + 2), *(macByte + 3), *(macByte + 4), *(macByte + 5));

    return macAddress;
}

char * getScreenType()
{
	bool isNew3DS = 0;
    APT_CheckNew3DS(&isNew3DS);
	
	static char uScreenType[20];
	static char dScreenType[20];
	
	static char screenType[32];
	
    if (isNew3DS)
    {
        u8 screens = 0;
        GSPLCD_GetVendors(&screens);
        switch ((screens >> 4) & 0xF)
        {
            case 1:
				sprintf(uScreenType, "Upper: IPS");
                break;
            case 0xC:
                sprintf(uScreenType, "Upper: TN");
                break;
            default:
                sprintf(uScreenType, "Upper: Unknown");
                break;
        }
        switch (screens & 0xF)
        {
            case 1:
                sprintf(dScreenType, " | Lower: IPS");
                break;
            case 0xC:
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

u64 principalIdToFriendCode(u64 pid)
{
	u64 fc = 0;
	
    frdPrincipalIdToFriendCode(&fc, pid);
    
	return fc;
}

u64 getLocalFriendCodeSeed()
{
	u64 seed = 0;
	
    PS_GetLocalFriendCodeSeed(&seed);
    
	return seed;
}

FriendKey getMyFriendKey(void)
{
    FriendKey fk;
    
	frdGetMyFriendKey(&fk);
    
	return fk;
}

char * getSerialNum(void)
{
	static char str[32];
    char serial[0x10];
	
    cfgsSecureInfoGetSerialNo(serial);
    strcpy(str, serial);
    
	return str;
}

u32 getDeviceId(void)
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
    return base64Encode(cert);
}

char * getNNID(void)
{
    static char tmp[0x11];
    ACT_GetAccountInfo(tmp, 0x11, 0x8);
	
    return tmp;
}