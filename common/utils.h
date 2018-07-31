#ifndef _3DSIDENT_UTILS_H_
#define _3DSIDENT_UTILS_H_

#include <3ds.h>

void Utils_GetSizeString(char *string, uint64_t size);
bool Utils_IsN3DS(void);
void Utils_U16_To_U8(char *buf, const u16 *input, size_t bufsize);
char *Utils_ExtractBetween(const char *string, const char *str1, const char *str2);
char *Utils_Base64Encode(u8 const *bytesToEnc, size_t bufLen);

#endif
