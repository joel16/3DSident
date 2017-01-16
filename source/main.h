#include <3ds.h>
#include <sf2d.h>
#include <sfil.h>
#include <sftd.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "Ubuntu_ttf.h"

#define NOT_FOUND_SIZE 32

#define load_PNG(texture, fname) { \
	if(access(fname, R_OK ) == -1 ) {\
		texture = sf2d_create_texture(NOT_FOUND_SIZE, NOT_FOUND_SIZE, TEXFMT_RGBA8, SF2D_PLACE_RAM); \
	} \
	else \
		texture = sfil_load_PNG_file(fname, SF2D_PLACE_RAM); \
	sf2d_texture_tile32(texture); \
}

#define selector_xDistance 0 //The distance between the X axis of the two selections
#define selector_yDistance 18 //The distance between the Y axis of the two selections

#define SDK(a,b,c,d) ((a<<24)|(b<<16)|(c<<8)|d)

char kernerlVersion[100], systemVersion[100], firmVersion[100], sdmcCID[36], nandCID[36];

sftd_font *font;
sf2d_texture *topScreen, *bottomScreen;