#pragma once
#include "Types.h"

class CCamera final {
public:
    static void Apply(float scale);
    static void Refresh();

private:
    typedef void (*TProjectionSetup)(Int32 depth);
};
