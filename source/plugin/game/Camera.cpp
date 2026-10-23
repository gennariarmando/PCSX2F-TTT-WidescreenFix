#include "game/Camera.h"

#include "Types.h"

#include "core/Patch.h"
#include "core/Mips.h"
#include "game/TekkenAddresses.h"

#include <string.h>

void CCamera::Apply(float scale) {
    UInt32 bits;
    memcpy(&bits, &scale, sizeof(bits));

    CPatch::Write32(CTekkenAddresses::CameraScaleFirst, CMips::Lui(CMips::At, bits >> 16));
    CPatch::Write32(CTekkenAddresses::CameraScaleSecond, CMips::Ori(CMips::At, CMips::At, bits & 0xFFFF));
    CPatch::Write32(CTekkenAddresses::CameraScaleThird, CMips::Mtc1(CMips::At, CMips::FloatRegister0));
    CPatch::Write32(CTekkenAddresses::CameraScaleFourth,
        CMips::MulS(CMips::FloatRegister24, CMips::FloatRegister24, CMips::FloatRegister0));
}

void CCamera::Refresh() {
    const Int32 depth = static_cast<Int32>(CPatch::Read32(CTekkenAddresses::ProjectionDepth));
    if (depth == 0)
        return;

    reinterpret_cast<TProjectionSetup>(CTekkenAddresses::ProjectionSetup)(depth);
}
