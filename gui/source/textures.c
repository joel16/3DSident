#include "sprites.h"
#include "textures.h"

static C2D_SpriteSheet spritesheet;

void Textures_Load(void)
{
	spritesheet = C2D_SpriteSheetLoad("romfs:/res/drawable/sprites.t3x");

	banner = C2D_SpriteSheetGetImage(spritesheet, sprites_icon_idx);
	drive_icon = C2D_SpriteSheetGetImage(spritesheet, sprites_drive_idx);
	btn_A = C2D_SpriteSheetGetImage(spritesheet, sprites_A_idx);
	btn_B = C2D_SpriteSheetGetImage(spritesheet, sprites_B_idx);
	btn_X = C2D_SpriteSheetGetImage(spritesheet, sprites_X_idx);
	btn_Y = C2D_SpriteSheetGetImage(spritesheet, sprites_Y_idx);
	btn_Start_Select = C2D_SpriteSheetGetImage(spritesheet, sprites_Start_select_idx);
	btn_L = C2D_SpriteSheetGetImage(spritesheet, sprites_L_idx);
	btn_R = C2D_SpriteSheetGetImage(spritesheet, sprites_R_idx);
	btn_ZL = C2D_SpriteSheetGetImage(spritesheet, sprites_ZL_idx);
	btn_ZR = C2D_SpriteSheetGetImage(spritesheet, sprites_ZR_idx);
	btn_Dpad = C2D_SpriteSheetGetImage(spritesheet, sprites_D_pad_idx);
	btn_Cpad = C2D_SpriteSheetGetImage(spritesheet, sprites_Circle_pad_idx);
	btn_Cstick = C2D_SpriteSheetGetImage(spritesheet, sprites_C_stick_idx);
	btn_home = C2D_SpriteSheetGetImage(spritesheet, sprites_Home_idx);
	cursor = C2D_SpriteSheetGetImage(spritesheet, sprites_cursor_idx);
	volumeIcon = C2D_SpriteSheetGetImage(spritesheet, sprites_volume_idx);
}

void Textures_Free(void)
{
	C2D_SpriteSheetFree(spritesheet);
}