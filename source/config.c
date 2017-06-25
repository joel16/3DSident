#include "config.h"

const char * getUsername() 
{
	int i;
	size_t size = 0x16;
	u8 * temp = (u8*)malloc(size);
	char * username = (char*)malloc(size / 2);
	
	for(i = 0; i < (size / 2); i++)
		username[i] = 0;
	
	CFGU_GetConfigInfoBlk2(0x1C, 0xA0000, temp);
	
	for(i = 0; i < (size / 2); i++)
		username[i] = (char)((u16*)temp)[i];
	
	return username;
}

char * getBirthday() 
{
	u16 date = 0;
	static char birthday[6];
	
	CFGU_GetConfigInfoBlk2(0x2, 0xA0001, (u8*)&date);
	
	u8 month = date / 256;
	u8 day = date % 256;
	
	sprintf(birthday, "%u/%u", day, month);
	
	return birthday;
}

char * getEulaVersion()
{
	u8 eulaData[4];
	static char version[5];
    
	CFGU_GetConfigInfoBlk2(4, 0xD0000, eulaData);
	
	sprintf(version, "%02X.%02X", eulaData[1], eulaData[0]);
	
	return version;
}