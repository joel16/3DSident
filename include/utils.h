#include <3ds.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ANY_KEY (KEY_TOUCH | KEY_A | KEY_B | KEY_X | KEY_Y | KEY_START | KEY_SELECT | \
		KEY_UP | KEY_DOWN | KEY_LEFT | KEY_RIGHT | KEY_L | KEY_R | KEY_ZL | KEY_ZR | \
		KEY_CSTICK_UP | KEY_CSTICK_DOWN | KEY_CSTICK_LEFT | KEY_CSTICK_RIGHT)

void getSizeString(char *string, uint64_t size);
char * base64Encode(u8 const * input);