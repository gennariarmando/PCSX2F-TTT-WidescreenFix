#pragma once
#include "Types.h"

class CDisplay final {
public:
    static void Start(float aspectOverride);
    static void Update();

private:
    static constexpr float RasterAspect = 4.0f / 3.0f;

    static float Aspect(Int32 setting, Int32 width, Int32 height);

    static inline float s_forcedAspect = 0.0f;
    static inline Int32 s_width = -1;
    static inline Int32 s_height = -1;
    static inline Int32 s_fullscreen = -1;
    static inline Int32 s_setting = -1;
    static inline bool s_active = false;
};
