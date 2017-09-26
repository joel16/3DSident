#include <stdio.h>

#include "hardware.h"

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
	
	if(R_SUCCEEDED(gspLcdInit()))
	{
		if (R_SUCCEEDED(GSPLCD_GetBrightness(screen, &brightness)))
			gspLcdExit();
	}
	
	snprintf(level, 5, "%d%%", (int)brightness);
	
	return level;
}