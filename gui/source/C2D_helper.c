#include <stdarg.h>

#include "common.h"
#include "C2D_helper.h"

void Draw_EndFrame(void)
{
	C2D_TextBufClear(dynamicBuf);
    C2D_TextBufClear(sizeBuf);
    C3D_FrameEnd(0);
}

void Draw_Text(float x, float y, float size, Colour colour, const char *text)
{
    C2D_Text c2d_text;
    C2D_TextParse(&c2d_text, dynamicBuf, text);
    C2D_TextOptimize(&c2d_text);
    C2D_DrawText(&c2d_text, C2D_WithColor, x, y, 0.5f, size, size, colour);
}

void Draw_Textf(float x, float y, float size, Colour colour, const char* text, ...)
{
    char buffer[256];
	va_list args;
	va_start(args, text);
	vsnprintf(buffer, 256, text, args);
	Draw_Text(x, y, size, colour, buffer);
	va_end(args);
}

void Draw_GetTextSize(float size, float *width, float *height, const char *text)
{
	C2D_Text c2d_text;
	C2D_TextParse(&c2d_text, sizeBuf, text);
	C2D_TextGetDimensions(&c2d_text, size, size, width, height);
}

float Draw_GetTextWidth(float size, const char *text)
{
	float width = 0;
	Draw_GetTextSize(size, &width, NULL, text);
	return width;
}

float Draw_GetTextHeight(float size, const char *text)
{
	float height = 0;
	Draw_GetTextSize(size, NULL, &height, text);
	return height;
}

bool Draw_Rect(float x, float y, float w, float h, Colour colour)
{
	return C2D_DrawRectSolid(x, y, 0.5f, w, h, colour);
}

bool Draw_Image(C2D_Image image, float x, float y)
{
	return C2D_DrawImageAt(image, x, y, 0.5f, NULL, 1.0f, 1.0f);
}

bool Draw_ImageScale(C2D_Image image, float x, float y, float scaleX, float scaleY)
{
	return C2D_DrawImageAt(image, x, y, 0.5f, NULL, scaleX, scaleY);
}

bool Draw_Image_Blend(C2D_Image image, float x, float y, Colour colour)
{
	C2D_ImageTint tint;
	C2D_PlainImageTint(&tint, colour, 0.50f);
	return C2D_DrawImageAt(image, x, y, 0.5f, &tint, 1.0f, 1.0f);
}
