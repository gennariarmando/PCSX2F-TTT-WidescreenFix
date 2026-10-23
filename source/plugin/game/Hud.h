#pragma once
#include "Types.h"

#include "game/HudTransform.h"
#include "game/HudTrampoline.h"

class CHud final {
public:
    static void Install(bool pillarboxFullWidth);
    static void SetScale(float scale);

private:
    static UInt32 Commit(UInt32 buffer, UInt32 end);
    static UInt32 BufferLimit(UInt32 buffer);

    alignas(16) static inline UInt32 s_trampolines[2][CHudTrampoline::Words]{};
    static inline CHudTransform::CSettings s_settings = { 0x10000, false };
};
