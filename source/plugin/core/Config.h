#pragma once

class CConfig final {
public:
    static void Load();
    static bool HudFix();
    static bool PillarboxFullWidth();
    static bool PillarboxStills();
    static bool SkipIntro();
    static float AspectOverride();

private:
    static bool ReadFlag(const char* key, bool fallback);
    static float ReadNumber(const char*& text);
    static float ParseAspect(const char* text);

    static inline bool s_hudFix = true;
    static inline bool s_pillarboxFullWidth = true;
    static inline bool s_pillarboxStills = true;
    static inline bool s_skipIntro = false;
    static inline float s_aspectOverride = 0.0f;
};
