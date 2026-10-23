#pragma once
#include "Types.h"

#include "core/Mips.h"
#include "game/TekkenAddresses.h"

class CIntroTrampoline final {
public:
    static constexpr UInt32 Words = 9;

    static void Build(UInt32 (&words)[Words], UInt32 movieStart) {
        words[0] = CMips::Addiu(CMips::Sp, CMips::Sp, -32);
        words[1] = CMips::Sd(CMips::Ra, 0, CMips::Sp);
        words[2] = CMips::Sd(CMips::A0, 8, CMips::Sp);
        words[3] = CMips::Jal(movieStart);
        words[4] = CMips::Nop();
        words[5] = CMips::Ld(CMips::Ra, 0, CMips::Sp);
        words[6] = CMips::Ld(CMips::A0, 8, CMips::Sp);
        words[7] = CMips::J(CTekkenAddresses::MovieStartResume);
        words[8] = CMips::Nop();
    }
};
