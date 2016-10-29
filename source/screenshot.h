#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <png.h>
#include <zlib.h>
#include <3ds.h>

#define NUM_LEVELS (Z_BEST_COMPRESSION - Z_NO_COMPRESSION + 1)
#define screenshotPath "/3ds/3DSident/screenshots/3DSident.png"

int level, screenCapture;
unsigned int format_choice;
GSPGPU_FramebufferFormats format; // = GSP_RGBA8_OES
void captureScreenshot();
