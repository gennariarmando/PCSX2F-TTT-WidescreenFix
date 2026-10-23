#include "game/FrameCopy.h"

#include "core/Patch.h"
#include "core/Mips.h"
#include "game/TekkenAddresses.h"

#include <string.h>

void CFrameCopy::Install(bool enable) {
    if (!enable)
        return;

    CDmaTrampoline::Build(s_trampoline, reinterpret_cast<UInt32>(&Kick));
    CPatch::Write32(CTekkenAddresses::DmaStart, CMips::J(reinterpret_cast<UInt32>(s_trampoline)));
}

void CFrameCopy::SetScale(float scale) {
    s_scale = static_cast<Int32>(scale * 65536.0f);
}

void CFrameCopy::NoteDraw() {
    s_draws++;
}

UInt32 CFrameCopy::Kick(UInt32 channel, UInt32 chain) {
    if (channel == CTekkenAddresses::Vif1Channel) {
        s_vifKicks++;
        return chain;
    }

    if (channel != CTekkenAddresses::GifChannel || (chain & 0xF0000000) != 0)
        return chain;

    const UInt8* tags = reinterpret_cast<const UInt8*>(chain);
    if (!CFrameCopyTransform::IsChain(tags))
        return chain;

    const UInt8* blob = reinterpret_cast<const UInt8*>(CFrameCopyTransform::ChainAddress(tags + 2 * CFrameCopyTransform::TagBytes));
    if (!CFrameCopyTransform::IsBlob(blob))
        return chain;

    const bool still = s_draws == 0 && s_vifKicks == 0;
    s_draws = 0;
    s_vifKicks = 0;

    if (!still || s_scale >= 0x10000)
        return chain;

    memcpy(s_blob, blob, CFrameCopyTransform::BlobBytes);
    CFrameCopyTransform::Squeeze(s_blob, s_scale);
    CFrameCopyTransform::BuildBars(s_bars, s_scale);
    CFrameCopyTransform::BuildChain(s_chain, tags, reinterpret_cast<UInt32>(s_blob), reinterpret_cast<UInt32>(s_bars));

    return reinterpret_cast<UInt32>(s_chain);
}
