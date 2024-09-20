#include "sprites.h"
#include "textures.h"

C2D_Image banner, driveIcon, menuIcon[10], btnA, btnB, btnX, btnY, btnStartSelect, btnL, btnR,
    btnZL, btnZR, btnDpad, btnCpad, btnCstick, btnHome, cursor, volumeIcon;

namespace Textures {
    static C2D_SpriteSheet spritesheet;
    
    void Init(void) {
        spritesheet = C2D_SpriteSheetLoad("romfs:/res/drawable/sprites.t3x");
        
        banner = C2D_SpriteSheetGetImage(spritesheet, sprites_icon_idx);
        driveIcon = C2D_SpriteSheetGetImage(spritesheet, sprites_drive_idx);

        // Menu items
        menuIcon[0] = C2D_SpriteSheetGetImage(spritesheet, sprites_kernel_idx);
        menuIcon[1] = C2D_SpriteSheetGetImage(spritesheet, sprites_system_idx);
        menuIcon[2] = C2D_SpriteSheetGetImage(spritesheet, sprites_power_idx);
        menuIcon[3] = C2D_SpriteSheetGetImage(spritesheet, sprites_nnid_idx);
        menuIcon[4] = C2D_SpriteSheetGetImage(spritesheet, sprites_config_idx);
        menuIcon[5] = C2D_SpriteSheetGetImage(spritesheet, sprites_controller_idx);
        menuIcon[6] = C2D_SpriteSheetGetImage(spritesheet, sprites_wifi_idx);
        menuIcon[7] = C2D_SpriteSheetGetImage(spritesheet, sprites_storage_idx);
        menuIcon[8] = C2D_SpriteSheetGetImage(spritesheet, sprites_misc_idx);
        menuIcon[9] = C2D_SpriteSheetGetImage(spritesheet, sprites_exit_idx);

        // Controls
        btnA = C2D_SpriteSheetGetImage(spritesheet, sprites_A_idx);
        btnB = C2D_SpriteSheetGetImage(spritesheet, sprites_B_idx);
        btnX = C2D_SpriteSheetGetImage(spritesheet, sprites_X_idx);
        btnY = C2D_SpriteSheetGetImage(spritesheet, sprites_Y_idx);
        btnStartSelect = C2D_SpriteSheetGetImage(spritesheet, sprites_start_select_idx);
        btnL = C2D_SpriteSheetGetImage(spritesheet, sprites_L_idx);
        btnR = C2D_SpriteSheetGetImage(spritesheet, sprites_R_idx);
        btnZL = C2D_SpriteSheetGetImage(spritesheet, sprites_ZL_idx);
        btnZR = C2D_SpriteSheetGetImage(spritesheet, sprites_ZR_idx);
        btnDpad = C2D_SpriteSheetGetImage(spritesheet, sprites_D_pad_idx);
        btnCpad = C2D_SpriteSheetGetImage(spritesheet, sprites_circle_pad_idx);
        btnCstick = C2D_SpriteSheetGetImage(spritesheet, sprites_C_stick_idx);
        btnHome = C2D_SpriteSheetGetImage(spritesheet, sprites_home_idx);
        cursor = C2D_SpriteSheetGetImage(spritesheet, sprites_cursor_idx);
        volumeIcon = C2D_SpriteSheetGetImage(spritesheet, sprites_volume_idx);
    }

    void Exit(void) {
        C2D_SpriteSheetFree(spritesheet);
    }
}
