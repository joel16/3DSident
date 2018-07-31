#ifndef _3DSIDENT_C2D_HELPER_H_
#define _3DSIDENT_C2D_HELPER_H_

#include <citro2d.h>

#define BACKGROUND_COLOUR      C2D_Color32(242, 241, 240, 255)
#define STATUS_BAR_COLOUR      C2D_Color32(69, 67, 62, 255)
#define MENU_BAR_COLOUR        C2D_Color32(255, 255, 255, 255)
#define ITEM_COLOUR            C2D_Color32(0, 0, 0, 255)
#define ITEM_SELECTED_COLOUR   MENU_BAR_COLOUR
#define MENU_SELECTOR_COLOUR   C2D_Color32(239, 118, 69, 255)
#define MENU_INFO_TITLE_COLOUR C2D_Color32(144, 137, 129, 255)
#define MENU_INFO_DESC_COLOUR  C2D_Color32(51, 51, 51, 255)

C3D_RenderTarget *RENDER_TOP, *RENDER_BOTTOM;
C2D_TextBuf staticBuf, dynamicBuf, sizeBuf;
C2D_Image banner, drive_icon;

typedef u32 Colour;

void Draw_EndFrame(void);
void Draw_Text(float x, float y, float size, Colour colour, const char *text);
void Draw_Textf(float x, float y, float size, Colour colour, const char* text, ...);
void Draw_GetTextSize(float size, float *width, float *height, const char *text);
float Draw_GetTextWidth(float size, const char *text);
float Draw_GetTextHeight(float size, const char *text);
bool Draw_Rect(float x, float y, float w, float h, Colour colour);
bool Draw_Image(C2D_Image image, float x, float y);

#endif