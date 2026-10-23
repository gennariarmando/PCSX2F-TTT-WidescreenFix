#include "core/Plugin.h"

#include "core/Config.h"
#include "core/Display.h"
#include "game/FrameCopy.h"
#include "game/Hud.h"
#include "game/Intro.h"

void CPlugin::Install() {
    CConfig::Load();
    CDisplay::Start(CConfig::AspectOverride());

    if (CConfig::HudFix())
        CHud::Install(CConfig::PillarboxFullWidth());

    CFrameCopy::Install(CConfig::PillarboxStills());

    if (CConfig::SkipIntro())
        CIntro::Install();
}
