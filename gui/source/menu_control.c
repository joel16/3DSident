#include <3ds.h>

#include "C2D_helper.h"
#include "menu_control.h"
#include "textures.h"

bool MENU_STATE_CONTROLS = false;

void Menu_Controls(void)
{
	circlePosition cPad, cStick;
	touchPosition touch;

	u16 touch_x = 0, touch_y = 0;

	u8 volume;

	int i = 0;
	
	while (MENU_STATE_CONTROLS) 
	{
		hidScanInput();
		hidCircleRead(&cPad);
		hidCstickRead(&cStick);
		u32 kDown = hidKeysDown();
		u32 kHeld = hidKeysHeld();
		HIDUSER_GetSoundVolume(&volume);

		if (((kHeld & KEY_START) && (kDown & KEY_SELECT)) || ((kHeld & KEY_SELECT) && (kDown & KEY_START)))
			MENU_STATE_CONTROLS = false;

		if (kHeld & KEY_TOUCH) 
		{
			hidTouchRead(&touch);
			touch_x = touch.px;
			touch_y = touch.py;
		}

		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(RENDER_TOP, C2D_Color32(60, 61, 63, 255));
		C2D_TargetClear(RENDER_BOTTOM, C2D_Color32(94, 39, 80, 255));
		C2D_SceneBegin(RENDER_TOP);

		Draw_Rect(75, 30, 250, 210, C2D_Color32(97, 101, 104, 255));
		Draw_Rect(85, 40, 230, 175, C2D_Color32(242, 241, 239, 255));
		Draw_Rect(85, 40, 230, 15, C2D_Color32(66, 65, 61, 255));
	
		Draw_Textf(90, 40, 0.45f, C2D_Color32(230, 232, 214, 255), "3DSident Button Test");
	
		Draw_Textf(90, 56, 0.45f, C2D_Color32(77, 76, 74, 255), "Circle pad: %04d, %04d", cPad.dx, cPad.dy);
		Draw_Textf(90, 70, 0.45f, C2D_Color32(77, 76, 74, 255), "C stick: %04d, %04d", cStick.dx, cStick.dy);
		Draw_Textf(90, 84, 0.45f, C2D_Color32(77, 76, 74, 255), "Touch position: %03d, %03d", touch.px, touch.py);
		Draw_Textf(90, 84, 0.45f, C2D_Color32(77, 76, 74, 255), "Touch position: %03d, %03d", touch.px, touch.py);
		
		Draw_Image(volumeIcon, 90, 98);
		double volPercent = (volume * 1.5873015873);
		Draw_Rect(115, 104, 190, 5, C2D_Color32(219, 219, 219, 255));
		Draw_Rect(115, 104, ((volPercent / 100) * 190), 5, C2D_Color32(241, 122, 74, 255));
		
		Draw_Textf(90, 118, 0.45f, C2D_Color32(77, 76, 74, 255), "3D");
		double _3dSliderPercent = (osGet3DSliderState() * 100.0);
		Draw_Rect(115, 122, 190, 5, C2D_Color32(219, 219, 219, 255));
		Draw_Rect(115, 122, ((_3dSliderPercent / 100) * 190), 5, C2D_Color32(241, 122, 74, 255));
		
		Draw_Textf(90, 138, 0.45f, C2D_Color32(77, 76, 74, 255), "Press START + SELECT to return.");
	
		Draw_Image(btn_home, 180, 215);
		
		if (kHeld & KEY_L)
			Draw_Image_Blend(btn_L, 0, 0, MENU_SELECTOR_COLOUR);
		else
			Draw_Image(btn_L, 0, 0);
	
		if (kHeld & KEY_R)
			Draw_Image_Blend(btn_R, 345, 0, MENU_SELECTOR_COLOUR);
		else
			Draw_Image(btn_R, 345, 0);
	
		if (kHeld & KEY_ZL)
			Draw_Image_Blend(btn_ZL, 60, 0, MENU_SELECTOR_COLOUR);
		else
			Draw_Image(btn_ZL, 60, 0);
	
		if (kHeld & KEY_ZR)
			Draw_Image_Blend(btn_ZR, 300, 0, MENU_SELECTOR_COLOUR);
		else
			Draw_Image(btn_ZR, 300, 0);
	
		if (kHeld & KEY_A)
			Draw_Image_Blend(btn_A, 370, 80, MENU_SELECTOR_COLOUR);
		else
			Draw_Image(btn_A, 370, 80);
	
		if (kHeld & KEY_B)
			Draw_Image_Blend(btn_B, 350, 100, MENU_SELECTOR_COLOUR);
		else
			Draw_Image(btn_B, 350, 100);
	
		if (kHeld & KEY_X)
			Draw_Image_Blend(btn_X, 350, 60, MENU_SELECTOR_COLOUR);
		else
			Draw_Image(btn_X, 350, 60);
	
		if (kHeld & KEY_Y)
			Draw_Image_Blend(btn_Y, 330, 80, MENU_SELECTOR_COLOUR);
		else
			Draw_Image(btn_Y, 330, 80);
	
		if (kHeld & KEY_START)
			Draw_Image_Blend(btn_Start_Select, 330, 140, MENU_SELECTOR_COLOUR);
		else
			Draw_Image(btn_Start_Select, 330, 140);
	
		if (kHeld & KEY_SELECT)
			Draw_Image_Blend(btn_Start_Select, 330, 165, MENU_SELECTOR_COLOUR);
		else
			Draw_Image(btn_Start_Select, 330, 165);
	
		if (kHeld & KEY_CPAD_LEFT)
			Draw_Image_Blend(btn_Cpad, 3, 55, MENU_SELECTOR_COLOUR);
		else if (kHeld & KEY_CPAD_RIGHT)
			Draw_Image_Blend(btn_Cpad, 13, 55, MENU_SELECTOR_COLOUR);
		else if (kHeld & KEY_CPAD_UP)
			Draw_Image_Blend(btn_Cpad, 8, 50, MENU_SELECTOR_COLOUR);
		else if (kHeld & KEY_CPAD_DOWN)
			Draw_Image_Blend(btn_Cpad, 8, 60, MENU_SELECTOR_COLOUR);
		else
			Draw_Image(btn_Cpad, 8, 55);
	
		if (kHeld & KEY_DLEFT)
			Draw_Image_Blend(btn_Dpad, 0, 110, MENU_SELECTOR_COLOUR);
		else if (kHeld & KEY_DRIGHT)
			Draw_Image_Blend(btn_Dpad, 10, 110, MENU_SELECTOR_COLOUR);
		else if (kHeld & KEY_DUP)
			Draw_Image_Blend(btn_Dpad, 5, 105, MENU_SELECTOR_COLOUR);
		else if (kHeld & KEY_DDOWN)
			Draw_Image_Blend(btn_Dpad, 5, 115, MENU_SELECTOR_COLOUR);
		else
			Draw_Image(btn_Dpad, 5, 110);
	
		if (kHeld & KEY_CSTICK_LEFT)
			Draw_Image_Blend(btn_Cstick, 325, 35, MENU_SELECTOR_COLOUR);
		else if (kHeld & KEY_CSTICK_RIGHT)
			Draw_Image_Blend(btn_Cstick, 335, 35, MENU_SELECTOR_COLOUR);
		else if (kHeld & KEY_CSTICK_UP)
			Draw_Image_Blend(btn_Cstick, 330, 30, MENU_SELECTOR_COLOUR);
		else if (kHeld & KEY_CSTICK_DOWN)
			Draw_Image_Blend(btn_Cstick, 330, 40, MENU_SELECTOR_COLOUR);
		else
			Draw_Image(btn_Cstick, 330, 35);

		C2D_SceneBegin(RENDER_BOTTOM);

		Draw_Image(cursor, touch_x, touch_y);

		Draw_EndFrame();
	}
}