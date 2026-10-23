#pragma once
#include "Types.h"

class CPatch final {
public:
    static void Write32(UInt32 address, UInt32 value) {
        *reinterpret_cast<volatile UInt32*>(address) = value;
    }

    static UInt32 Read32(UInt32 address) {
        return *reinterpret_cast<const volatile UInt32*>(address);
    }
};
