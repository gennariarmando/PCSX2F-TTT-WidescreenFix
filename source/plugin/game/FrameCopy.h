#pragma once
#include "Types.h"

#include "game/DmaTrampoline.h"
#include "game/FrameCopyTransform.h"

class CFrameCopy final {
public:
    static void Install(bool enable);
    static void SetScale(float scale);
    static void NoteDraw();

private:
    static UInt32 Kick(UInt32 channel, UInt32 chain);

    alignas(16) static inline UInt32 s_trampoline[CDmaTrampoline::Words]{};
    alignas(16) static inline UInt8 s_chain[4 * CFrameCopyTransform::TagBytes]{};
    alignas(16) static inline UInt8 s_blob[CFrameCopyTransform::BlobBytes]{};
    alignas(16) static inline UInt8 s_bars[CFrameCopyTransform::BarBytes]{};
    static inline Int32 s_scale = 0x10000;
    static inline UInt32 s_draws = 0;
    static inline UInt32 s_vifKicks = 0;
};
