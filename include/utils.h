#include <3ds.h>
#include <sf2d.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void setBilinearFilter(int enabled, sf2d_texture *texture);
void getSizeString(char *string, uint64_t size);
char * base64Encode(u8 const * input);