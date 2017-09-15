#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"

void getSizeString(char *string, uint64_t size) //Thanks TheOfficialFloW
{
	double double_size = (double)size;

	int i = 0;
	static char *units[] = { "B", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB" };
	while (double_size >= 1024.0f) {
		double_size /= 1024.0f;
		i++;
	}

	sprintf(string, "%.*f %s", (i == 0) ? 0 : 2, double_size, units[i]);
}

bool isN3DS(void)
{
	bool isNew3DS = false;
	
	if (R_SUCCEEDED(APT_CheckNew3DS(&isNew3DS)))
		return isNew3DS;
	
	return false;
}

void u16_to_u8(char * buf, const u16 * input, size_t bufsize)
{
	ssize_t units = utf16_to_utf8((u8 *)buf, input, bufsize);
	
	if (units < 0) 
		units = 0;
	
	buf[units] = 0;
}

// Crashes doesn't work. Leavign it here for anyone who's interested.
// Also, this is Rei's function (initially in C++, in working condition) not mine.
static const char * base64_chars = 
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"abcdefghijklmnopqrstuvwxyz"
	"0123456789+/";

char * base64Encode(u8 const * bytesToEnc, size_t bufLen) 
{
	char * ret = "";
	int i = 0, j = 0;
	u8 temp[3];
	u8 str[4];

	while (bufLen--) 
	{
		temp[i++] = *(bytesToEnc++);
		if (i == 3) 
		{
			str[0] = (temp[0] & 0xfc) >> 2;
			str[1] = ((temp[0] & 0x03) << 4) + ((temp[1] & 0xf0) >> 4);
			str[2] = ((temp[1] & 0x0f) << 2) + ((temp[2] & 0xc0) >> 6);
			str[3] = temp[2] & 0x3f;

			for(i = 0; (i <4) ; i++) ret += base64_chars[str[i]];
				i = 0;
		}
	}

	if (i)
	{
		for(j = i; j < 3; j++) temp[j] = '\0';

		str[0] = (temp[0] & 0xfc) >> 2;
		str[1] = ((temp[0] & 0x03) << 4) + ((temp[1] & 0xf0) >> 4);
		str[2] = ((temp[1] & 0x0f) << 2) + ((temp[2] & 0xc0) >> 6);
		str[3] = temp[2] & 0x3f;

		for (j = 0; (j < i + 1); j++) 
			ret += base64_chars[str[j]];

		while((i++ < 3)) 
			ret += '=';
	}

	return ret;
}