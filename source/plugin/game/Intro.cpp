#include "game/Intro.h"

#include "Types.h"

#include "core/Patch.h"
#include "core/Mips.h"
#include "game/TekkenAddresses.h"

void CIntro::Install() {
    s_originalPoll = CPatch::Read32(CTekkenAddresses::MovieSkipPoll);
    s_originalLoop = CPatch::Read32(CTekkenAddresses::MovieSkipLoop);

    CIntroTrampoline::Build(s_trampoline, reinterpret_cast<UInt32>(&OnMovieStart));
    CPatch::Write32(CTekkenAddresses::MovieStart, CMips::J(reinterpret_cast<UInt32>(s_trampoline)));
}

void CIntro::OnMovieStart(UInt32 id) {
    (void)id;

    Apply(!s_introSeen);
    s_introSeen = true;
}

void CIntro::Apply(bool skip) {
    const UInt32 forced = CMips::Addiu(CMips::V0, CMips::Zero, 1);
    CPatch::Write32(CTekkenAddresses::MovieSkipPoll, skip ? CMips::Addiu(CMips::V1, CMips::Zero, 1) : s_originalPoll);
    CPatch::Write32(CTekkenAddresses::MovieSkipLoop, skip ? forced : s_originalLoop);
}
