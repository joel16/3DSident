#include <stdio.h>
#include <string.h>

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
   
	if (R_SUCCEEDED(CFGU_GetSystemModel(&model)))
	{
		if (model < 6)
			return models[model];
	}
	
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
	
	if (R_SUCCEEDED(CFGU_SecureInfoGetRegion(&region)))
	{
		if (region < 7)
			return regions[region];
	}
	
	return regions[7];
}

const char getFirmRegion(void)
{
	if (strncmp(getRegion(), "JPN", 3) == 0)
		return 'J';
	else if (strncmp(getRegion(), "USA", 3) == 0)
		return 'U';
	else if ((strncmp(getRegion(), "EUR", 3) == 0) || (strncmp(getRegion(), "AUS", 3) == 0))
		return 'E';
	else if (strncmp(getRegion(), "CHN", 3) == 0)
		return 'C';
	else if (strncmp(getRegion(), "KOR", 3) == 0)
		return 'K';
	else if (strncmp(getRegion(), "TWN", 3) == 0)
		return 'T';
	
	return 0;
}

bool IsCoppacsSupported()
{
	u8 IsCoppacs = 0;
	
	if (R_SUCCEEDED(CFGU_GetRegionCanadaUSA(&IsCoppacs)))
	{
		if (IsCoppacs)
			return true;
	}
	
	return false;
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
		"Traditional Chinese",
		"Unknown"
	};

	u8 language;
	
	if (R_SUCCEEDED(CFGU_GetSystemLanguage(&language)))
	{
		if (language < 12)
			return languages[language];
	}
	
	return languages[12];
}

char * getMacAddress(void)
{
	u8 * macByte = (u8 *)0x1FF81060; 
	static char macAddress[18];
	
	snprintf(macAddress, 18, "%02X:%02X:%02X:%02X:%02X:%02X", *macByte, *(macByte + 1), *(macByte + 2), *(macByte + 3), *(macByte + 4), *(macByte + 5));

	return macAddress;
}

char * getScreenType(void)
{	
	static char uScreenType[20];
	static char dScreenType[20];
	
	static char screenType[32];
	
	if (isN3DS())
	{
		u8 screens = 0;
		
		if(R_SUCCEEDED(gspLcdInit()))
		{
			if (R_SUCCEEDED(GSPLCD_GetVendors(&screens)))
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
	
    if (R_SUCCEEDED(PS_GetLocalFriendCodeSeed(&seed)))
		return seed;
	
	return 0;
}

u8 * getSerialNumber(void)
{
    static u8 serial[0xF];
	
    if (R_SUCCEEDED(CFGS_SecureInfoGetSerialNo(serial)))
		return serial;
    
	return NULL;
}

u64 getSoapId(void)
{
    u32 id = 0;
	
    if (R_SUCCEEDED(AM_GetDeviceId(&id)))
		return (id | (((u64) 4) << 32));
	
	return 0;
}