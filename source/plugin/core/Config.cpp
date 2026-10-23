#include "core/Config.h"

#include "Types.h"

#include "core/Api.h"
#include "ps2/inireader.h"

void CConfig::Load() {
    const UInt32 size = *reinterpret_cast<const UInt32*>(PluginData);
    inireader.SetIniPath(PluginData + sizeof(UInt32), size);

    char aspect[32] = {};
    inireader.ReadString("MAIN", "AspectRatio", "Auto", aspect, sizeof(aspect));
    s_aspectOverride = ParseAspect(aspect);
    s_hudFix = ReadFlag("HudFix", true);
    s_pillarboxFullWidth = ReadFlag("PillarboxFullWidth", true);
    s_pillarboxStills = ReadFlag("PillarboxStills", true);
    s_skipIntro = ReadFlag("SkipIntro", false);

}

bool CConfig::ReadFlag(const char* key, bool fallback) {
    return inireader.ReadInteger("MAIN", key, fallback ? 1 : 0) != 0;
}

float CConfig::ReadNumber(const char*& text) {
    while (*text == ' ')
        text++;

    float value = 0.0f;
    while (*text >= '0' && *text <= '9')
        value = value * 10.0f + static_cast<float>(*text++ - '0');

    if (*text == '.') {
        text++;
        float scale = 0.1f;
        while (*text >= '0' && *text <= '9') {
            value += scale * static_cast<float>(*text++ - '0');
            scale *= 0.1f;
        }
    }

    while (*text == ' ')
        text++;
    return value;
}

float CConfig::ParseAspect(const char* text) {
    const float width = ReadNumber(text);
    if (*text != ':' && *text != '/')
        return width;

    text++;
    const float height = ReadNumber(text);
    return height > 0.0f ? width / height : 0.0f;
}

bool CConfig::HudFix() {
    return s_hudFix;
}

bool CConfig::PillarboxFullWidth() {
    return s_pillarboxFullWidth;
}

bool CConfig::PillarboxStills() {
    return s_pillarboxStills;
}

bool CConfig::SkipIntro() {
    return s_skipIntro;
}

float CConfig::AspectOverride() {
    return s_aspectOverride;
}
