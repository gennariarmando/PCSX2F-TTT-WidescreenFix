#include "game/Hud.h"

#include "core/Display.h"
#include "core/Patch.h"
#include "core/Mips.h"
#include "game/FrameCopy.h"
#include "game/TekkenAddresses.h"

void CHud::Install(bool pillarboxFullWidth) {
    s_settings.pillarboxTextured = pillarboxFullWidth;

    const UInt32 commit = reinterpret_cast<UInt32>(&Commit);
    CHudTrampoline::Build(s_trampolines[0], commit, 0, CMips::S3, CTekkenAddresses::HudCommitBase, CTekkenAddresses::HudFirstCommitResume);
    CHudTrampoline::Build(s_trampolines[1], commit, 1, CMips::S4, CTekkenAddresses::HudCommitBase, CTekkenAddresses::HudSecondCommitResume);

    CPatch::Write32(CTekkenAddresses::HudFirstCommit, CMips::J(reinterpret_cast<UInt32>(s_trampolines[0])));
    CPatch::Write32(CTekkenAddresses::HudSecondCommit, CMips::J(reinterpret_cast<UInt32>(s_trampolines[1])));
}

void CHud::SetScale(float scale) {
    s_settings.scale = static_cast<Int32>(scale * 65536.0f);
}

UInt32 CHud::Commit(UInt32 buffer, UInt32 end) {
    CDisplay::Update();
    CFrameCopy::NoteDraw();

    const UInt32 pointer = CTekkenAddresses::HudBufferState +
        (buffer ? CTekkenAddresses::HudSecondBufferPointer : CTekkenAddresses::HudFirstBufferPointer);
    const UInt32 begin = CPatch::Read32(pointer);
    if (end <= begin)
        return end;

    const UInt8* limit = reinterpret_cast<const UInt8*>(BufferLimit(buffer));
    UInt8* newEnd = CHudTransform::TransformRecords(reinterpret_cast<UInt8*>(begin), reinterpret_cast<UInt8*>(end), limit, s_settings);
    return reinterpret_cast<UInt32>(newEnd);
}

UInt32 CHud::BufferLimit(UInt32 buffer) {
    if (buffer == 0) {
        const UInt32 base = CPatch::Read32(CTekkenAddresses::HudFirstBufferBase) +
            (CPatch::Read32(CTekkenAddresses::HudBufferState + CTekkenAddresses::HudFirstBufferIndex) << CTekkenAddresses::HudFirstBufferShift);
        return base + CTekkenAddresses::HudFirstBufferSize;
    }

    const UInt32 base = CPatch::Read32(CTekkenAddresses::HudSecondBufferBase) +
        CPatch::Read32(CTekkenAddresses::HudBufferState + CTekkenAddresses::HudSecondBufferIndex) * CTekkenAddresses::HudSecondBufferStride;
    return base + CTekkenAddresses::HudSecondBufferSize;
}
