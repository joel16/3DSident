#include <3ds.h>
#include <citro2d.h>
#include <cstdarg>
#include <cstdio>
#include <malloc.h>

#include "config.h"
#include "gui.h"
#include "hardware.h"
#include "service.h"
#include "textures.h"
#include "utils.h"

namespace GUI {
    enum {
        TARGET_TOP = 0,
        TARGET_BOTTOM,
        TARGET_MAX
    };

    enum PageState {
        KERNEL_INFO_PAGE = 0,
        SYSTEM_INFO_PAGE,
        BATTERY_INFO_PAGE,
        NNID_INFO_PAGE,
        CONFIG_INFO_PAGE,
        HARDWARE_INFO_PAGE,
        WIFI_INFO_PAGE,
        STORAGE_INFO_PAGE,
        MISC_INFO_PAGE,
        EXIT_PAGE,
        MAX_ITEMS
    };

    static C3D_RenderTarget *c3dRenderTarget[TARGET_MAX];
    static C2D_TextBuf guiStaticBuf, guiDynamicBuf, guiSizeBuf;

    static const u32 guiBgcolour = C2D_Color32(62, 62, 62, 255);
    static const u32 guiStatusBarColour = C2D_Color32(44, 44, 44, 255);
    static const u32 guiMenuBarColour = C2D_Color32(52, 52, 52, 255);
    static const u32 guiSelectorColour = C2D_Color32(223, 74, 22, 255);
    static const u32 guiTitleColour = C2D_Color32(252, 252, 252, 255);
    static const u32 guiDescrColour = C2D_Color32(182, 182, 182, 255);
    
    static const u32 guiItemDistance = 20, guiItemHeight = 18, guiItemStartX = 15, guiItemStartY = 84;
    static const float guiTexSize = 0.5f;

    void Init(void) {
        romfsInit();
        gfxInitDefault();
        C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
        C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
        C2D_Prepare();

        c3dRenderTarget[TARGET_TOP] = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
        c3dRenderTarget[TARGET_BOTTOM] = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

        guiStaticBuf  = C2D_TextBufNew(4096);
        guiDynamicBuf  = C2D_TextBufNew(4096);
        guiSizeBuf = C2D_TextBufNew(4096);

        Textures::Init();

        // Real time services
#if !defined BUILD_CITRA
        mcuHwcInit();
#endif
        ptmuInit();
        socInit((u32*)memalign(0x1000, 0x10000), 0x10000);
    }

    void Exit(void) {
        socExit();
        ptmuExit();
#if !defined BUILD_CITRA
        mcuHwcExit();
#endif
        Textures::Exit();
        C2D_TextBufDelete(guiSizeBuf);
        C2D_TextBufDelete(guiDynamicBuf);
        C2D_TextBufDelete(guiStaticBuf);
        C2D_Fini();
        C3D_Fini();
        gfxExit();
        romfsExit();
    }
    
    static void Begin(u32 topScreenColour, u32 bottomScreenColour) {
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C2D_TargetClear(c3dRenderTarget[TARGET_TOP], topScreenColour);
        C2D_TargetClear(c3dRenderTarget[TARGET_BOTTOM], bottomScreenColour);
        C2D_SceneBegin(c3dRenderTarget[TARGET_TOP]);
    }

    static void End(void) {
        C2D_TextBufClear(guiDynamicBuf);
        C2D_TextBufClear(guiSizeBuf);
        C3D_FrameEnd(0);
    }

    static void GetTextDimensions(float size, float *width, float *height, const char *text) {
        C2D_Text c2dText;
        C2D_TextParse(&c2dText, guiSizeBuf, text);
        C2D_TextGetDimensions(&c2dText, size, size, width, height);
    }

    static void DrawText(float x, float y, float size, u32 colour, const char *text) {
        C2D_Text c2dText;
        C2D_TextParse(&c2dText, guiDynamicBuf, text);
        C2D_TextOptimize(&c2dText);
        C2D_DrawText(&c2dText, C2D_WithColor, x, y, guiTexSize, size, size, colour);
    }

    static void DrawTextf(float x, float y, float size, u32 colour, const char* text, ...) {
        char buffer[128];
        va_list args;
        va_start(args, text);
        std::vsnprintf(buffer, 128, text, args);
        GUI::DrawText(x, y, size, colour, buffer);
        va_end(args);
    }

    static void DrawItem(float x, float y, const char *title, const char *text) {
        float titleWidth = 0.f;
        GUI::GetTextDimensions(guiTexSize, &titleWidth, nullptr, title);
        GUI::DrawText(x, y, guiTexSize, guiTitleColour, title);
        GUI::DrawText(x + titleWidth + 5, y, guiTexSize, guiDescrColour, text);
    }

    static void DrawItem(int index, const char *title, const char *text) {
        float titleWidth = 0.f;
        float y = guiItemStartY + ((guiItemDistance - guiItemHeight) / 2) + guiItemHeight * index;
        GUI::GetTextDimensions(guiTexSize, &titleWidth, nullptr, title);
        GUI::DrawText(guiItemStartX, y, guiTexSize, guiTitleColour, title);
        GUI::DrawText(guiItemStartX + titleWidth + 5, y, guiTexSize, guiDescrColour, text);
    }

    static void DrawItemf(int index, const char *title, const char *text, ...) {
        float titleWidth = 0.f;
        float y = guiItemStartY + ((guiItemDistance - guiItemHeight) / 2) + guiItemHeight * index;
        GUI::GetTextDimensions(guiTexSize, &titleWidth, nullptr, title);
        GUI::DrawText(guiItemStartX, y, guiTexSize, guiTitleColour, title);
        
        char buffer[256];
        va_list args;
        va_start(args, text);
        std::vsnprintf(buffer, 256, text, args);
        GUI::DrawText(guiItemStartX + titleWidth + 5, y, guiTexSize, guiDescrColour, buffer);
        va_end(args);
    }

    static bool DrawImage(C2D_Image image, float x, float y) {
        return C2D_DrawImageAt(image, x, y, guiTexSize, nullptr, 1.f, 1.f);
    }

    static bool DrawImageBlend(C2D_Image image, float x, float y, u32 colour) {
        C2D_ImageTint tint;
        C2D_PlainImageTint(std::addressof(tint), colour, 0.5f);
        return C2D_DrawImageAt(image, x, y, guiTexSize, std::addressof(tint), 1.f, 1.f);
    }

    static void KernelInfoPage(const KernelInfo &info, bool &displayInfo) {
        GUI::DrawItemf(1, "Kernel version:", info.kernelVersion);
        GUI::DrawItem(2, "FIRM version:", info.firmVersion);
        GUI::DrawItemf(3, "System version:", info.systemVersion);
        GUI::DrawItemf(4, "Initial system version:", info.initialVersion);
        GUI::DrawItemf(5, "SDMC CID:", displayInfo? info.sdmcCid : "");
        GUI::DrawItemf(6, "NAND CID:", displayInfo? info.nandCid : "");
        GUI::DrawItemf(7, "Device ID:", "%lu", displayInfo? info.deviceId : 0);
    }

    static void SystemInfoPage(const SystemInfo &info, bool &displayInfo) {
        GUI::DrawItemf(1, "Model:", "%s (%s - %s)", info.model, info.hardware, info.region);
        GUI::DrawItem(2, "Language:", info.language);
        GUI::DrawItemf(3, "Original local friend code seed:", "%010llX", displayInfo? info.localFriendCodeSeed : 0);
        GUI::DrawItemf(4, "NAND local friend code seed:", "%010llX", displayInfo? info.localFriendCodeSeed : 0);
        GUI::DrawItemf(5, "MAC Address:", displayInfo? info.macAddress : "");
        GUI::DrawItemf(6, "Serial number:", displayInfo? reinterpret_cast<const char *>(info.serialNumber) : "");
    }

    static void BatteryInfoPage(void) {
        Result ret = 0;
        u8 percentage = 0, status = 0, voltage = 0, fwVerHigh = 0, fwVerLow = 0, temp = 0;
        bool connected = false;

        ret = MCUHWC_GetBatteryLevel(std::addressof(percentage));
        GUI::DrawItemf(1, "Battery percentage:", "%3d%%", R_FAILED(ret)? 0 : (percentage));

        ret = PTMU_GetBatteryChargeState(std::addressof(status));
        GUI::DrawItem(2, "Battery status:", R_FAILED(ret)? "unknown" : (status? "charging" : "not charging"));

        ret = MCUHWC_GetBatteryVoltage(std::addressof(voltage));
        GUI::DrawItemf(3, "Battery voltage:", "%d (%.1f V)", voltage, 5.f * (static_cast<float>(voltage) / 256.f));

        ret = MCUHWC::GetBatteryTemperature(std::addressof(temp));
        GUI::DrawItemf(4, "Battery temperature:", "%d", R_FAILED(ret)? 0 : (temp));

        ret = PTMU_GetAdapterState(std::addressof(connected));
        GUI::DrawItemf(5, "Adapter state:", R_FAILED(ret)? "unknown" : (connected? "connected" : "disconnected"));

        ret = MCUHWC_GetFwVerHigh(std::addressof(fwVerHigh));
        ret = MCUHWC_GetFwVerLow(std::addressof(fwVerLow));
        GUI::DrawItemf(6, "MCU firmware:", "%u.%u", (fwVerHigh - 0x10), fwVerLow);
        
        GUI::DrawItem(7, "Power-saving mode:", Config::GetPowersaveStatus());
    }

    static void NNIDInfoPage(const NNIDInfo &info, bool &displayInfo) {
        GUI::DrawItemf(1, "Persistent ID:", "%u", displayInfo? info.persistentID : 0);
        GUI::DrawItemf(2, "Transferable ID Base:", "%llu", displayInfo? info.transferableIdBase : 0);
        GUI::DrawItem(3, "Account ID:", info.accountId);
        GUI::DrawItem(4, "Country:", displayInfo? info.countryName : "");
        GUI::DrawItemf(5, "Principal ID:", "%u", displayInfo? info.principalID : 0);
        GUI::DrawItem(6, "NFS Password:", displayInfo? info.nfsPassword : "");
    }

    static void ConfigInfoPage(const ConfigInfo &info, bool &displayInfo) {
        GUI::DrawItem(1, "Username:", info.username);
        GUI::DrawItem(2, "Birthday:", displayInfo? info.birthday : "");
        GUI::DrawItem(3, "EULA version:", info.eulaVersion);
        GUI::DrawItem(4, "Parental control pin:", displayInfo? info.parentalPin : "");
        GUI::DrawItem(5, "Parental control email:", displayInfo? info.parentalEmail : "");
        GUI::DrawItem(6, "Parental control answer:", displayInfo? info.parentalSecretAnswer : "");
    }

    static void HardwareInfoPage(const HardwareInfo &info, bool &isNew3DS) {
        GUI::DrawItem(1, "Upper screen type:", info.screenUpper);
        GUI::DrawItem(2, "Lower screen type:", info.screenLower);
        GUI::DrawItem(3, "Headphone status:", Hardware::GetAudioJackStatus()? "inserted" : "not inserted");
        GUI::DrawItem(4, "Card slot status:", Hardware::GetCardSlotStatus()? "inserted" : "not inserted");
        GUI::DrawItem(5, "SD status:", Hardware::IsSdInserted()? "inserted" : "not inserted");
        GUI::DrawItem(6, "Sound output:", info.soundOutputMode);
        
        if (isNew3DS) {
            GUI::DrawItemf(7, "Brightness level:", "%lu (auto-brightness mode: %s)", Hardware::GetBrightness(GSPLCD_SCREEN_TOP), 
                Hardware::GetAutoBrightnessStatus());
        }
        else {
            GUI::DrawItemf(7, "Brightness level:", "%lu", Hardware::GetBrightness(GSPLCD_SCREEN_TOP));
        }
    }

    static void WifiInfoPage(const WifiInfo &info, bool &displayInfo) {
        const u32 slotDistance = 68;
        C2D_DrawRectSolid(0, 20, guiTexSize, 400, 220, guiBgcolour);

        for (int i = 0; i < 3; i++) {
            if (info.slot[i]) {
                C2D_DrawRectSolid(15, 27 + (i * slotDistance), guiTexSize, 370, 70, guiTitleColour);
                C2D_DrawRectSolid(16, 28 + (i * slotDistance), guiTexSize, 368, 68, guiStatusBarColour);
                GUI::DrawTextf(20, 30 + (i * slotDistance), guiTexSize, guiTitleColour, "WiFi Slot %d:", i + 1);
                GUI::DrawTextf(20, 46 + (i * slotDistance), guiTexSize, guiTitleColour, "SSID: %s", info.ssid[i]);
                GUI::DrawTextf(20, 62 + (i * slotDistance), guiTexSize, guiTitleColour, "Pass: %s (%s)",
                    displayInfo? info.passphrase[i] : "", info.securityMode[i]);
            }
        }
    }

    static void StorageInfoPage(const StorageInfo &info) {
        C2D_DrawRectSolid(0, 20, guiTexSize, 400, 220, guiBgcolour);

        // SD info
        C2D_DrawRectSolid(20, 105, guiTexSize, 60, 10, guiTitleColour);
        C2D_DrawRectSolid(21, 106, guiTexSize, 58, 8, guiBgcolour);
        C2D_DrawRectSolid(21, 106, guiTexSize, ((static_cast<double>(info.usedSize[SYSTEM_MEDIATYPE_SD]) / static_cast<double>(info.totalSize[SYSTEM_MEDIATYPE_SD])) * 58.f), 8, guiSelectorColour);
        GUI::DrawItem(85, 50, "SD:", "");
        GUI::DrawItem(85, 71, "Free:", info.freeSizeString[SYSTEM_MEDIATYPE_SD]);
        GUI::DrawItem(85, 87, "Used:", info.usedSizeString[SYSTEM_MEDIATYPE_SD]);
        GUI::DrawItem(85, 103, "Total:", info.totalSizeString[SYSTEM_MEDIATYPE_SD]);
        GUI::DrawImage(driveIcon, 20, 40);
        
        // Nand info
        C2D_DrawRectSolid(220, 105, guiTexSize, 60, 10, guiTitleColour);
        C2D_DrawRectSolid(221, 106, guiTexSize, 58, 8, guiBgcolour);
        C2D_DrawRectSolid(221, 106, guiTexSize, ((static_cast<double>(info.usedSize[SYSTEM_MEDIATYPE_CTR_NAND]) / static_cast<double>(info.totalSize[SYSTEM_MEDIATYPE_CTR_NAND])) * 58.f), 8, guiSelectorColour);
        GUI::DrawItem(285, 50, "CTR Nand:", "");
        GUI::DrawItem(285, 71, "Free:", info.freeSizeString[SYSTEM_MEDIATYPE_CTR_NAND]);
        GUI::DrawItem(285, 87, "Used:", info.usedSizeString[SYSTEM_MEDIATYPE_CTR_NAND]);
        GUI::DrawItem(285, 103, "Total:", info.totalSizeString[SYSTEM_MEDIATYPE_CTR_NAND]);
        GUI::DrawImage(driveIcon, 220, 40);
        
        // TWL nand info
        C2D_DrawRectSolid(20, 200, guiTexSize, 60, 10, guiTitleColour);
        C2D_DrawRectSolid(21, 201, guiTexSize, 58, 8, guiBgcolour);
        C2D_DrawRectSolid(21, 201, guiTexSize, ((static_cast<double>(info.usedSize[SYSTEM_MEDIATYPE_TWL_NAND]) / static_cast<double>(info.totalSize[SYSTEM_MEDIATYPE_TWL_NAND])) * 58.f), 8, guiSelectorColour);
        GUI::DrawItem(85, 145, "TWL Nand:", "");
        GUI::DrawItem(85, 166, "Free:", info.freeSizeString[SYSTEM_MEDIATYPE_TWL_NAND]);
        GUI::DrawItem(85, 182, "Used:", info.usedSizeString[SYSTEM_MEDIATYPE_TWL_NAND]);
        GUI::DrawItem(85, 198, "Total:", info.totalSizeString[SYSTEM_MEDIATYPE_TWL_NAND]);
        GUI::DrawImage(driveIcon, 20, 135);

        // TWL photo info
        C2D_DrawRectSolid(220, 200, guiTexSize, 60, 10, guiTitleColour);
        C2D_DrawRectSolid(221, 201, guiTexSize, 58, 8, guiBgcolour);
        C2D_DrawRectSolid(221, 201, guiTexSize, ((static_cast<double>(info.usedSize[SYSTEM_MEDIATYPE_TWL_PHOTO]) / static_cast<double>(info.totalSize[SYSTEM_MEDIATYPE_TWL_PHOTO])) * 58.f), 8, guiSelectorColour);
        GUI::DrawItem(285, 145, "TWL Photo:", "");
        GUI::DrawItem(285, 166, "Free:", info.freeSizeString[SYSTEM_MEDIATYPE_TWL_PHOTO]);
        GUI::DrawItem(285, 182, "Used:", info.usedSizeString[SYSTEM_MEDIATYPE_TWL_PHOTO]);
        GUI::DrawItem(285, 198, "Total:", info.totalSizeString[SYSTEM_MEDIATYPE_TWL_PHOTO]);
        GUI::DrawImage(driveIcon, 220, 135);
    }

    static void MiscInfoPage(const MiscInfo &info, bool &displayInfo) {
        GUI::DrawItemf(1, "Installed titles:", "SD: %lu (NAND: %lu)", info.sdTitleCount, info.nandTitleCount);
        GUI::DrawItemf(2, "Installed tickets:", "%lu", info.ticketCount);

        u8 wifiStrength = osGetWifiStrength();
        GUI::DrawItemf(3, "WiFi signal strength:", "%lu", "%d (%.0lf%%)", wifiStrength, static_cast<float>(wifiStrength * 33.33));
        
        char hostname[128];
        gethostname(hostname, sizeof(hostname));
        GUI::DrawItem(4, "IP:", displayInfo? hostname : "");
    }

    void ButtonTester(bool &enabled) {
        circlePosition circlePad, cStick;
        touchPosition touch;
        u16 touchX = 0, touchY = 0;
        u8 volume = 0;
        
        const u32 guiButtonTesterText = C2D_Color32(77, 76, 74, 255);
        const u32 guiButtonTesterSliderBorder = C2D_Color32(219, 219, 219, 255);
        const u32 guiButtonTesterSlider = C2D_Color32(241, 122, 74, 255);
        
        while (enabled) {
            hidScanInput();
            
            hidCircleRead(std::addressof(circlePad));
            hidCstickRead(std::addressof(cStick));
            
            u32 kDown = hidKeysDown();
            u32 kHeld = hidKeysHeld();
            
            HIDUSER_GetSoundVolume(std::addressof(volume));
            
            if (((kHeld & KEY_L) && (kDown & KEY_R)) || ((kHeld & KEY_R) && (kDown & KEY_L))) {
                enabled = false;
            }
            
            if (kHeld & KEY_TOUCH)  {
                hidTouchRead(&touch);
                touchX = touch.px;
                touchY = touch.py;
            }

            GUI::Begin(C2D_Color32(60, 61, 63, 255), C2D_Color32(94, 39, 80, 255));
            
            C2D_DrawRectSolid(75, 30, guiTexSize, 250, 210, C2D_Color32(97, 101, 104, 255));
            C2D_DrawRectSolid(85, 40, guiTexSize, 230, 175, C2D_Color32(242, 241, 239, 255));
            C2D_DrawRectSolid(85, 40, guiTexSize, 230, 15, C2D_Color32(66, 65, 61, 255));
            
            GUI::DrawText(90, 40, 0.45f, guiTitleColour, "3DSident Button Test");
            
            GUI::DrawTextf(90, 56, 0.45f, guiButtonTesterText, "Circle pad: %04d, %04d", circlePad.dx, circlePad.dy);
            GUI::DrawTextf(90, 70, 0.45f, guiButtonTesterText, "C stick: %04d, %04d", cStick.dx, cStick.dy);
            GUI::DrawTextf(90, 84, 0.45f, guiButtonTesterText, "Touch position: %03d, %03d", touch.px, touch.py);
            
            GUI::DrawImage(volumeIcon, 90, 98);
            double volPercent = (volume * 1.5873015873);
            C2D_DrawRectSolid(115, 104, guiTexSize, 190, 5, guiButtonTesterSliderBorder);
            C2D_DrawRectSolid(115, 104, guiTexSize, ((volPercent / 100) * 190), 5, guiButtonTesterSlider);
            
            GUI::DrawText(90, 118, 0.45f, guiButtonTesterText, "3D");
            double _3dSliderPercent = (osGet3DSliderState() * 100.0);
            C2D_DrawRectSolid(115, 122, guiTexSize, 190, 5, guiButtonTesterSliderBorder);
            C2D_DrawRectSolid(115, 122, guiTexSize, ((_3dSliderPercent / 100) * 190), 5, guiButtonTesterSlider);
            
            GUI::DrawText(90, 138, 0.45f, guiButtonTesterText, "Press L + R to return.");
            
            GUI::DrawImage(btnHome, 180, 215);

            kHeld & KEY_L? DrawImageBlend(btnL, 0, 0, guiSelectorColour) : GUI::DrawImage(btnL, 0, 0);
            kHeld & KEY_R? DrawImageBlend(btnR, 345, 0, guiSelectorColour) : GUI::DrawImage(btnR, 345, 0);
            kHeld & KEY_ZL? DrawImageBlend(btnZL, 60, 0, guiSelectorColour) : GUI::DrawImage(btnZL, 60, 0);
            kHeld & KEY_ZR? DrawImageBlend(btnZR, 300, 0, guiSelectorColour) : GUI::DrawImage(btnZR, 300, 0);
            kHeld & KEY_A? DrawImageBlend(btnA, 370, 80, guiSelectorColour) : GUI::DrawImage(btnA, 370, 80);
            kHeld & KEY_B? DrawImageBlend(btnB, 350, 100, guiSelectorColour) : GUI::DrawImage(btnB, 350, 100);
            kHeld & KEY_X? DrawImageBlend(btnX, 350, 60, guiSelectorColour) : GUI::DrawImage(btnX, 350, 60);
            kHeld & KEY_Y? DrawImageBlend(btnY, 330, 80, guiSelectorColour) : GUI::DrawImage(btnY, 330, 80);
            kHeld & KEY_START? DrawImageBlend(btnStartSelect, 330, 140, guiSelectorColour) : GUI::DrawImage(btnStartSelect, 330, 140);
            kHeld & KEY_SELECT? DrawImageBlend(btnStartSelect, 330, 165, guiSelectorColour) : GUI::DrawImage(btnStartSelect, 330, 165);
            
            if (kHeld & KEY_CPAD_LEFT) {
                DrawImageBlend(btnCpad, 3, 55, guiSelectorColour);
            }
            else if (kHeld & KEY_CPAD_RIGHT) {
                DrawImageBlend(btnCpad, 13, 55, guiSelectorColour);
            }
            else if (kHeld & KEY_CPAD_UP) {
                DrawImageBlend(btnCpad, 8, 50, guiSelectorColour);
            }
            else if (kHeld & KEY_CPAD_DOWN) {
                DrawImageBlend(btnCpad, 8, 60, guiSelectorColour);
            }
            else {
                GUI::DrawImage(btnCpad, 8, 55);
            }
            
            if (kHeld & KEY_DLEFT) {
                DrawImageBlend(btnDpad, 0, 110, guiSelectorColour);
            }
            else if (kHeld & KEY_DRIGHT) {
                DrawImageBlend(btnDpad, 10, 110, guiSelectorColour);
            }
            else if (kHeld & KEY_DUP) {
                DrawImageBlend(btnDpad, 5, 105, guiSelectorColour);
            }
            else if (kHeld & KEY_DDOWN) {
                DrawImageBlend(btnDpad, 5, 115, guiSelectorColour);
            }
            else {
                GUI::DrawImage(btnDpad, 5, 110);
            }
            
            if (kHeld & KEY_CSTICK_LEFT) {
                DrawImageBlend(btnCstick, 325, 35, guiSelectorColour);
            }
            else if (kHeld & KEY_CSTICK_RIGHT) {
                DrawImageBlend(btnCstick, 335, 35, guiSelectorColour);
            }
            else if (kHeld & KEY_CSTICK_UP) {
                DrawImageBlend(btnCstick, 330, 30, guiSelectorColour);
            }
            else if (kHeld & KEY_CSTICK_DOWN) {
                DrawImageBlend(btnCstick, 330, 40, guiSelectorColour);
            }
            else {
                GUI::DrawImage(btnCstick, 330, 35);
            }
            
            C2D_SceneBegin(c3dRenderTarget[TARGET_BOTTOM]);
            GUI::DrawImage(cursor, touchX, touchY);
            GUI::End();
        }
    }

    void MainMenu(void) {
        int selection = 0;
        bool isNew3DS = Utils::IsNew3DS(), displayInfo = true, buttonTestEnabled = false;

        const char *items[] = {
            "Kernel",
            "System",
            "Battery",
            "NNID",
            "Config",
            "Hardware",
            "Wi-Fi",
            "Storage",
            "Miscellaneous",
            "Exit"
        };

        float titleHeight = 0.f;
        GUI::GetTextDimensions(guiTexSize, nullptr, &titleHeight, "3DSident v0.0.0");
        
        KernelInfo kernelInfo = { 0 };
        SystemInfo systemInfo = { 0 };
        NNIDInfo nnidInfo = { 0 };
        ConfigInfo configInfo = { 0 };
        HardwareInfo hardwareInfo = { 0 };
        WifiInfo wifiInfo = { 0 };
        StorageInfo storageInfo = { 0 };
        MiscInfo miscInfo = { 0 };

        Service::Init();
        kernelInfo = Service::GetKernelInfo();
        systemInfo = Service::GetSystemInfo();
        nnidInfo = Service::GetNNIDInfo();
        configInfo = Service::GetConfigInfo();
        hardwareInfo = Service::GetHardwareInfo();
        wifiInfo = Service::GetWifiInfo();
        storageInfo = Service::GetStorageInfo();
        miscInfo = Service::GetMiscInfo();
        Service::Exit();

        while (aptMainLoop()) {
            GUI::Begin(guiBgcolour, guiBgcolour);

            C2D_DrawRectSolid(0, 0, guiTexSize, 400, 20, guiStatusBarColour);
            GUI::DrawTextf(5, (20 - titleHeight) / 2, guiTexSize, guiTitleColour, "3DSident v%d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_MICRO);
            GUI::DrawImage(banner, (400 - banner.subtex->width) / 2, ((82 - banner.subtex->height) / 2) + 20);

            switch (selection) {
                case KERNEL_INFO_PAGE:
                    GUI::KernelInfoPage(kernelInfo, displayInfo);
                    break;

                case SYSTEM_INFO_PAGE:
                    GUI::SystemInfoPage(systemInfo, displayInfo);
                    break;

                case BATTERY_INFO_PAGE:
                    GUI::BatteryInfoPage();
                    break;

                case NNID_INFO_PAGE:
                    GUI::NNIDInfoPage(nnidInfo, displayInfo);
                    break;

                case CONFIG_INFO_PAGE:
                    GUI::ConfigInfoPage(configInfo, displayInfo);
                    break;

                case HARDWARE_INFO_PAGE:
                    GUI::HardwareInfoPage(hardwareInfo, isNew3DS);
                    break;

                case WIFI_INFO_PAGE:
                    GUI::WifiInfoPage(wifiInfo, displayInfo);
                    break;

                case STORAGE_INFO_PAGE:
                    GUI::StorageInfoPage(storageInfo);
                    break;

                case MISC_INFO_PAGE:
                    GUI::MiscInfoPage(miscInfo, displayInfo);
                    break;

                case EXIT_PAGE:
                    GUI::DrawItem(1, "Press select to hide user-specific info.", "");
                    GUI::DrawItem(2, "Press L + R to use button tester.", "");
                    break;

                default:
                    break;
            }

            C2D_SceneBegin(c3dRenderTarget[TARGET_BOTTOM]);
            
            C2D_DrawRectSolid(15, 15, guiTexSize, 290, 210, guiTitleColour);
            C2D_DrawRectSolid(16, 16, guiTexSize, 288, 208, guiMenuBarColour);
            C2D_DrawRectSolid(16, 16 + (guiItemDistance * selection), guiTexSize, 288, 18, guiSelectorColour);

            for (int i = 0; i < MAX_ITEMS; i++) {
                C2D_DrawImageAt(menuIcon[i], 20, 17 + ((guiItemDistance - guiItemHeight) / 2) + (guiItemDistance * i), guiTexSize, nullptr, 0.7f, 0.7f);
                GUI::DrawText(40, 17 + ((guiItemDistance - guiItemHeight) / 2) + (guiItemDistance * i), guiTexSize, guiTitleColour, items[i]);
            }
            
            GUI::End();
            GUI::ButtonTester(buttonTestEnabled);

            hidScanInput();
            u32 kDown = hidKeysDown();
            u32 kHeld = hidKeysHeld();

            if (kDown & KEY_DOWN) {
                selection++;
            }
            else if (kDown & KEY_UP) {
                selection--;
            }

            if (selection > EXIT_PAGE) {
                selection = KERNEL_INFO_PAGE;
            }
            if (selection < KERNEL_INFO_PAGE) {
                selection = EXIT_PAGE;
            }

            if (kDown & KEY_SELECT) {
                displayInfo = !displayInfo;
            }

            if (((kHeld & KEY_L) && (kDown & KEY_R)) || ((kHeld & KEY_R) && (kDown & KEY_L))) {
                buttonTestEnabled = true;
            }

            if ((kDown & KEY_START) || ((kDown & KEY_A) && (selection == EXIT_PAGE))) {
                break;
            }
        }
    }
}
