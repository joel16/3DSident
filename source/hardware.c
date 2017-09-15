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
	
	if(R_SUCCEEDED(gspLcdInit()))
	{
		if (R_SUCCEEDED(GSPLCD_GetBrightness(screen, &brightness)))
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