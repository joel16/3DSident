#include <stdio.h>

#include "hardware.h"

#define REG_LCD_TOP_SCREEN (u32)0x202200
#define REG_LCD_BOTTOM_SCREEN (u32)0x202A00

char * getCardSlotStatus(void)
{
	bool isInserted = false;
	FS_CardType cardType = 0;
	
	static char card[20];
	
	if (R_SUCCEEDED(FSUSER_CardSlotIsInserted(&isInserted)))
	{
		if (isInserted)
		{
			FSUSER_GetCardType(&cardType);
			snprintf(card, 20, "inserted %s", cardType? "(TWL)" : "(CTR)");
			return card;
		}
	}
	
	snprintf(card, 20, "not inserted");
	return card;
}

bool detectSD(void)
{
	bool isSD = false;
	
	if (R_SUCCEEDED(FSUSER_IsSdmcDetected(&isSD)))
		return isSD;
	
	return false;
}

char * getBrightness(u32 screen)
{
	u32 brightness = 0;
	static char level[5];

	if (R_SUCCEEDED(gspInit()))
	{
		if (screen == GSPLCD_SCREEN_TOP)
		{
			if (R_SUCCEEDED(GSPGPU_ReadHWRegs(REG_LCD_TOP_SCREEN + 0x40, &brightness, 4)))
				gspExit();
		}
		else if (screen = GSPLCD_SCREEN_BOTTOM)
		{
			if (R_SUCCEEDED(GSPGPU_ReadHWRegs(REG_LCD_BOTTOM_SCREEN + 0x40, &brightness, 4)))
				gspExit();
		}
		else
			gspExit();
	}

	snprintf(level, 0x4, "%d", (int)brightness);
	
	return level;
}