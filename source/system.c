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

	CFG_Region region = 0;
	
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

	CFG_Language language;
	
	if (R_SUCCEEDED(CFGU_GetSystemLanguage(&language)))
	{
		if (language < 12)
			return languages[language];
	}
	
	return languages[12];
}

char * getMacAddress(void)
{
	u8 * macByte = (u8 *)WIFI_MACADDR; 
	static char macAddress[18];
	
	snprintf(macAddress, 18, "%02X:%02X:%02X:%02X:%02X:%02X", *macByte, *(macByte + 1), *(macByte + 2), *(macByte + 3), *(macByte + 4), *(macByte + 5));

	return macAddress;
}

char * getRunningHW(void)
{
	u8 * data = (u8 *)RUNNING_HW; 
	static char runningHW[0x9];
	
	switch (*data)
	{
		case 0x1:
			snprintf(runningHW, 0x7, "Retail");
			break;
		case 0x2:
			snprintf(runningHW, 0x9, "Devboard");
			break;
		case 0x3:
			snprintf(runningHW, 0x9, "Debugger");
			break;
		case 0x4:
			snprintf(runningHW, 0x8, "Capture");
			break;
	}

	return runningHW;
}

char * isDebugUnit(void)
{
	return *(char *)0x1FF80015 ? "(Debug Unit)" : "";
}

char * getScreenType(void)
{	
	static char upperScreen[20];
	static char lowerScreen[20];
	
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
			case 0x01: // 0x01 = JDI => IPS
				sprintf(upperScreen, "Upper: IPS");
				break;
			case 0x0C: // 0x0C = SHARP => TN
				sprintf(upperScreen, "Upper: TN");
				break;
			default:
				sprintf(upperScreen, "Upper: Unknown");
				break;
		}
		switch (screens & 0xF)
		{
			case 0x01: // 0x01 = JDI => IPS
				sprintf(lowerScreen, " | Lower: IPS");
				break;
			case 0x0C: // 0x0C = SHARP => TN
				sprintf(lowerScreen, " | Lower: TN");
				break;
			default:
				sprintf(lowerScreen, " | Lower: Unknown");
				break;
		}
		
		strcpy(screenType, upperScreen);
		strcat(screenType, lowerScreen);
	}
	else
		sprintf(screenType, "Upper: TN | Lower: TN"); 
	
	return screenType;
}

u64 getLocalFriendCodeSeed(void)
{
	u64 seed = 0;
	
    	if (R_SUCCEEDED(CFGI_GetLocalFriendCodeSeed(&seed)))
		return seed;
	
	return 0;
}

u8 * getSerialNumber(void)
{
    static u8 serial[0xF];
	
    if (R_SUCCEEDED(CFGI_SecureInfoGetSerialNumber(serial)))
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
