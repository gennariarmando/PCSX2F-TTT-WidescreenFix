#pragma once
#include "Types.h"

#include "core/Mips.h"
#include "game/TekkenAddresses.h"

class CDmaTrampoline final {
public:
    static constexpr UInt32 Words = 14;

    static void Build(UInt32 (&words)[Words], UInt32 kick) {
        words[0] = CMips::Addiu(CMips::Sp, CMips::Sp, -32);
        words[1] = CMips::Sd(CMips::Ra, 0, CMips::Sp);
        words[2] = CMips::Sd(CMips::A0, 8, CMips::Sp);
        words[3] = CMips::Sd(CMips::A1, 16, CMips::Sp);
        words[4] = CMips::Jal(kick);
        words[5] = CMips::Nop();
        words[6] = CMips::Ld(CMips::Ra, 0, CMips::Sp);
        words[7] = CMips::Ld(CMips::A0, 8, CMips::Sp);
        words[8] = CMips::Move(CMips::A1, CMips::V0);
        words[9] = CMips::Addiu(CMips::Sp, CMips::Sp, 32);
        words[10] = CMips::Lui(CMips::V0, 0x1000);
        words[11] = CMips::Lui(CMips::V1, 0x8FFF);
        words[12] = CMips::J(CTekkenAddresses::DmaStartResume);
        words[13] = CMips::Nop();
    }
};
