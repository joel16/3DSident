#ifndef UTILS_H
#define UTILS_H

#include <3ds.h>

#define ANY_KEY (KEY_TOUCH | KEY_A | KEY_B | KEY_X | KEY_Y | KEY_START | KEY_SELECT | \
		KEY_UP | KEY_DOWN | KEY_LEFT | KEY_RIGHT | KEY_L | KEY_R | KEY_ZL | KEY_ZR | \
		KEY_CSTICK_UP | KEY_CSTICK_DOWN | KEY_CSTICK_LEFT | KEY_CSTICK_RIGHT)

void getSizeString(char * string, uint64_t size);
bool isN3DS(void);
void u16_to_u8(char * buf, const u16 * input, size_t bufsize);
char * base64Encode(u8 const * bytesToEnc, size_t bufLen);

#endif