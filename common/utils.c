#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"

void Utils_GetSizeString(char *string, uint64_t size) //Thanks TheOfficialFloW
{
	double double_size = (double)size;

	int i = 0;
	static char *units[] = { "B", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB" };

	while (double_size >= 1024.0f)
	{
		double_size /= 1024.0f;
		i++;
	}

	sprintf(string, "%.*f %s", (i == 0) ? 0 : 2, double_size, units[i]);
}

bool Utils_IsN3DS(void)
{
	bool isNew3DS = false;

	if (R_SUCCEEDED(APT_CheckNew3DS(&isNew3DS)))
		return isNew3DS;

	return false;
}

void Utils_U16_To_U8(char *buf, const u16 *input, size_t bufsize)
{
	ssize_t units = utf16_to_utf8((u8 *)buf, input, bufsize);

	if (units < 0)
		units = 0;
	
	buf[units] = 0;
}

void Utils_U8_To_U16(u16 *buf, const char *input, size_t bufsize)
{
	ssize_t units = utf8_to_utf16(buf, (const uint8_t*)input, bufsize);

	if (units < 0)
		units = 0;

	buf[units] = 0;
}

char *Utils_ExtractBetween(const char *string, const char *str1, const char *str2)
{
	const char *i1 = strstr(string, str1);
	
	if (i1 != NULL)
	{
    	const size_t pl1 = strlen(str1);
    	const char *i2 = strstr(i1 + pl1, str2);
    
    	if (str2 != NULL)
    	{
     		/* Found both markers, extract text. */
     		const size_t mlen = i2 - (i1 + pl1);
     		char *ret = malloc(mlen + 1);
     		
     		if (ret != NULL)
     		{
       			memcpy(ret, i1 + pl1, mlen);
       			ret[mlen] = '\0';
       			return ret;
     		}
    	}
   	}

  	return "";
}

// Crashes doesn't work. Leavign it here for anyone who's interested.
// Also, this is Rei's function (initially in C++, in working condition) not mine.
static const char *base64_chars = 
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"abcdefghijklmnopqrstuvwxyz"
	"0123456789+/";

char *Utils_Base64Encode(u8 const *bytesToEnc, size_t bufLen) 
{
	char *ret = "";
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
