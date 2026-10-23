#pragma once
#include "Types.h"

#include "core/Mips.h"

class CHudTrampoline final {
public:
    static constexpr UInt32 Words = 12;

    static void Build(UInt32 (&words)[Words], UInt32 commit, UInt32 buffer, UInt32 baseRegister, UInt32 baseUpper, UInt32 resume) {
        words[0] = CMips::Addiu(CMips::Sp, CMips::Sp, -32);
        words[1] = CMips::Sd(CMips::Ra, 0, CMips::Sp);
        words[2] = CMips::Move(CMips::A1, CMips::A0);
        words[3] = CMips::Addiu(CMips::A0, CMips::Zero, static_cast<Int32>(buffer));
        words[4] = CMips::Jal(commit);
        words[5] = CMips::Nop();
        words[6] = CMips::Ld(CMips::Ra, 0, CMips::Sp);
        words[7] = CMips::Move(CMips::A0, CMips::V0);
        words[8] = CMips::Addiu(CMips::Sp, CMips::Sp, 32);
        words[9] = CMips::Lui(baseRegister, baseUpper);
        words[10] = CMips::J(resume);
        words[11] = CMips::Nop();
    }
};
