#include <stdio.h>

#include "hardware.h"
#include "utils.h"

#define REG_LCD_TOP_SCREEN (u32)0x202200
#define REG_LCD_BOTTOM_SCREEN (u32)0x202A00

char *Hardware_GetAudioJackStatus(void)
{
	bool audio_jack = false;
	static char status[0xD];

	if (R_SUCCEEDED(FSUSER_CardSlotIsInserted(&audio_jack)))
	{
		snprintf(status, 0xD, audio_jack? "inserted" : "not inserted");
		return status;
	}

	return NULL;
}

char *Hardware_GetCardSlotStatus(void)
{
	bool isInserted = false;
	FS_CardType cardType = 0;
	
	static char card[0x14];
	
	if (R_SUCCEEDED(FSUSER_CardSlotIsInserted(&isInserted)))
	{
		if (isInserted)
		{
			FSUSER_GetCardType(&cardType);
			snprintf(card, 0x14, "inserted %s", cardType? "(TWL)" : "(CTR)");
			return card;
		}
		else
		{
			snprintf(card, 0x14, "not inserted");
			return card;
		}
	}

	return NULL;	
}

char *Hardware_DetectSD(void)
{
	bool detect = false;
	static char status[0xD];

	if (R_SUCCEEDED(FSUSER_IsSdmcDetected(&detect)))
	{
		snprintf(status, 0xD, detect? "detected" : "not detected");
		return status;
	}

	return NULL;
}

char *Hardware_GetBrightness(u32 screen)
{
	u32 brightness = 0;
	static char level[5];

	Result ret = 0;

	if (R_SUCCEEDED(ret = gspInit()))
	{
		if (screen == GSPLCD_SCREEN_TOP)
		{
			if (R_SUCCEEDED(ret = GSPGPU_ReadHWRegs(REG_LCD_TOP_SCREEN + 0x40, &brightness, 4)))
				gspExit();
		}
		else if (screen = GSPLCD_SCREEN_BOTTOM)
		{
			if (R_SUCCEEDED(ret = GSPGPU_ReadHWRegs(REG_LCD_BOTTOM_SCREEN + 0x40, &brightness, 4)))
				gspExit();
		}
		else
			gspExit();
	}

	snprintf(level, 0x4, "%lu", R_FAILED(ret)? 0 : brightness);
	
	return level;
}