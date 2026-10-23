#pragma once
#include "Types.h"

class CMips final {
public:
    enum ERegister : UInt32 {
        Zero = 0,
        At = 1,
        V0 = 2,
        V1 = 3,
        A0 = 4,
        A1 = 5,
        S3 = 19,
        S4 = 20,
        Sp = 29,
        Ra = 31,
    };

    static constexpr UInt32 FloatRegister0 = 0;
    static constexpr UInt32 FloatRegister24 = 24;

    static constexpr UInt32 Nop() {
        return 0;
    }

    static constexpr UInt32 Addiu(UInt32 rt, UInt32 rs, Int32 immediate) {
        return Immediate(0x09, rs, rt, immediate);
    }

    static constexpr UInt32 Lui(UInt32 rt, UInt32 immediate) {
        return Immediate(0x0F, Zero, rt, static_cast<Int32>(immediate));
    }

    static constexpr UInt32 Ori(UInt32 rt, UInt32 rs, UInt32 immediate) {
        return Immediate(0x0D, rs, rt, static_cast<Int32>(immediate));
    }

    static constexpr UInt32 Ld(UInt32 rt, Int32 offset, UInt32 base) {
        return Immediate(0x37, base, rt, offset);
    }

    static constexpr UInt32 Sd(UInt32 rt, Int32 offset, UInt32 base) {
        return Immediate(0x3F, base, rt, offset);
    }

    static constexpr UInt32 Bnez(UInt32 rs, Int32 wordOffset) {
        return Immediate(0x05, rs, Zero, wordOffset);
    }

    static constexpr UInt32 Move(UInt32 rd, UInt32 rs) {
        return (rs << 21) | (rd << 11) | 0x25;
    }

    static constexpr UInt32 J(UInt32 target) {
        return 0x08000000 | Target(target);
    }

    static constexpr UInt32 Jal(UInt32 target) {
        return 0x0C000000 | Target(target);
    }

    static constexpr UInt32 Mtc1(UInt32 rt, UInt32 fs) {
        return 0x44800000 | (rt << 16) | (fs << 11);
    }

    static constexpr UInt32 MulS(UInt32 fd, UInt32 fs, UInt32 ft) {
        return 0x46000002 | (0x10 << 21) | (ft << 16) | (fs << 11) | (fd << 6);
    }

private:
    static constexpr UInt32 Immediate(UInt32 opcode, UInt32 rs, UInt32 rt, Int32 immediate) {
        return (opcode << 26) | (rs << 21) | (rt << 16) | (static_cast<UInt32>(immediate) & 0xFFFF);
    }

    static constexpr UInt32 Target(UInt32 address) {
        return (address & 0x0FFFFFFF) >> 2;
    }
};
