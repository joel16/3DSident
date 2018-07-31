#ifndef _3DSIDENT_TEXTURES_H_
#define _3DSIDENT_TEXTURES_H_

#include <citro2d.h>

C2D_Image banner, drive_icon;
C2D_Image btn_A, btn_B, btn_X, btn_Y, btn_Start_Select, btn_L, btn_R, btn_ZL, btn_ZR, btn_Dpad, btn_Cpad, btn_Cstick, btn_home, cursor, volumeIcon;

void Textures_Load(void);
void Textures_Free(void);

#endif