#include "core/Display.h"

#include "core/Api.h"
#include "game/Camera.h"
#include "game/FrameCopy.h"
#include "game/Hud.h"

void CDisplay::Start(float aspectOverride) {
    s_forcedAspect = aspectOverride;
    Update();
}

void CDisplay::Update() {
    Int32 width = PCSX2Data[PCSX2Data_WindowSizeX];
    Int32 height = PCSX2Data[PCSX2Data_WindowSizeY];
    const Int32 fullscreen = PCSX2Data[PCSX2Data_IsFullscreen];
    const Int32 setting = PCSX2Data[PCSX2Data_AspectRatioSetting];

    if (fullscreen || width <= 0 || height <= 0) {
        width = PCSX2Data[PCSX2Data_DesktopSizeX];
        height = PCSX2Data[PCSX2Data_DesktopSizeY];
    }

    if (width == s_width && height == s_height && fullscreen == s_fullscreen && setting == s_setting)
        return;

    s_width = width;
    s_height = height;
    s_fullscreen = fullscreen;
    s_setting = setting;

    const float scale = RasterAspect / Aspect(setting, width, height);
    CCamera::Apply(scale);
    CHud::SetScale(scale);
    CFrameCopy::SetScale(scale);

    if (s_active)
        CCamera::Refresh();
    s_active = true;
}

float CDisplay::Aspect(Int32 setting, Int32 width, Int32 height) {
    if (s_forcedAspect > 0.0f)
        return s_forcedAspect;

    switch (setting) {
    case RAuto4_3_3_2:
    case R4_3:
        return 4.0f / 3.0f;
    case R16_9:
        return 16.0f / 9.0f;
    default:
        break;
    }

    if (width <= 0 || height <= 0)
        return RasterAspect;
    return static_cast<float>(width) / static_cast<float>(height);
}
