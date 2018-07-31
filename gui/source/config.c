#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "config.h"
#include "utils.h"

struct Birthday
{
    s8  month;  // birthday month (1 - 12)
    s8  day;    // birthday day (1 - 31)
};

const wchar_t *Config_GetUsername(void)
{
	u8 data[0x1C];
	static wchar_t userName[0x13];
    
	if (R_SUCCEEDED(CFGU_GetConfigInfoBlk2(0x1C, 0x000A0000, data)))
	{
		for (int i = 0; i < 0x13; i++)
			userName[i] = (wchar_t)((u16 *)data)[i];

		return userName;
	}

	return NULL;
}

char *Config_GetBirthday(void)
{
	u8 data[0x2];
	static char date[0xA];

	struct Birthday birthday;
	
	if (R_SUCCEEDED(CFGU_GetConfigInfoBlk2(0x2, 0x000A0001, data)))
	{
		birthday.month = data[0x01];
		birthday.day = data[0x00];
	
		snprintf(date, 0xA, "%02d/%02d", birthday.day, birthday.month);
		return date;
	}
	
	return NULL;
}

char *Config_GetEulaVersion(void)
{
	u8 data[0x4];
	static char version[0x6];
    
	if (R_SUCCEEDED(CFGU_GetConfigInfoBlk2(0x4, 0x000D0000, data)))
	{
		snprintf(version, 0x6, "%1X.%02X", data[0x1], data[0x0]);
		return version;
	}

	return NULL;
}

char *Config_GetSoundOutputMode(void)
{
	u8 data[0x1];
	
	static char *mode[] = 
	{
		"mono",
		"stereo",
		"surround"
	};
    
	if (R_SUCCEEDED(CFGU_GetConfigInfoBlk2(0x1, 0x00070001, data)))
		return mode[data[0x0]];
	
	return NULL;
}

char *Config_GetParentalPin(void)
{	
	u8 data[0x94];
	static char parentalPin[0x5];
	
	if (R_SUCCEEDED(CFG_GetConfigInfoBlk8(0x94, 0x00100001, data)))
	{
		snprintf(parentalPin, 0x5, "%u%u%u%u", (data[0xD] - 0x30), (data[0xE] - 0x30), (data[0xF] - 0x30), (data[0x10] - 0x30));
		return parentalPin;
	}

	return NULL;
}

char *Config_GetParentalEmail(void)
{	
	u8 data[0x200];
	static char email[0x200];
	
	if (R_SUCCEEDED(CFGU_GetConfigInfoBlk2(0x200, 0x000C0002, data)))
	{
		snprintf(email, 0x200, "%s", (data + 1));
		return email;
	}

	return NULL;
}

char *Config_GetParentalSecretAnswer(void)
{	
	u8 data[0x94]; // block 0x00100001 is of size 0x94
	static char out[0x21];
	
	if (R_SUCCEEDED(CFG_GetConfigInfoBlk8(0x94, 0x00100001, data)))
	{
		Utils_U16_To_U8(out, (u16 *)(data + 0x10), 0x21); // 0x21 cause the secret answer can only be 32 characters long.
		return out + 1;
	}

	return NULL;
}

bool Config_IsDebugModeEnabled(void)
{
	u8 data[0x4];
	
	if (R_SUCCEEDED(CFGU_GetConfigInfoBlk2(0x4, 0x00130000, data)))
	{
		if ((data[0x0] + data[0x1] + data[0x2] + data[0x3]) == 0x100)
			return true;
	}
	
	return false;
}

bool Config_IsUpdatesEnabled(void)
{
	u8 data[0x4];
	bool isEnabled = false;
    
	if (R_SUCCEEDED(CFG_GetConfigInfoBlk8(0x4, 0x000F0005, data)))
	{
		isEnabled = data[0] & 0xFF;
		return isEnabled;
	}
	
	return false;
}

/*
	u8 data[0x2];
	data[0x0] -> u8 ABL_powersave_enable
	data[0x1] -> u8 brightness_level
*/

bool Config_IsPowerSaveEnabled(void)
{
	u8 data[0x2];
	bool isEnabled = false;
    
	if (R_SUCCEEDED(CFG_GetConfigInfoBlk8(0x2, 0x00050001, data)))
	{
		isEnabled = data[0] & 0xFF;
		return isEnabled;
	}
	
	return false;
}

bool Config_IsAutoBrightnessEnabled(void)
{
	u8 data[0x8];
	bool isEnabled = false;
    
	if (R_SUCCEEDED(CFG_GetConfigInfoBlk8(0x8, 0x00050009, data)))
	{
		isEnabled = data[0x4] & 0xFF;
		return isEnabled;
	}

	return false;
}
