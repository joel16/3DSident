#pragma once

#include <citro2d.h>

extern C2D_Image banner, driveIcon, menuIcon[10], btnA, btnB, btnX, btnY, btnStartSelect, btnL, btnR,
    btnZL, btnZR, btnDpad, btnCpad, btnCstick, btnHome, cursor, volumeIcon;

namespace Textures {
    void Init(void);
    void Exit(void);
}
