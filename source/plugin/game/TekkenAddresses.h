#pragma once
#include "Types.h"

class CTekkenAddresses final {
public:
    static constexpr UInt32 DiscCrc = 0x67454C1E;

    static constexpr UInt32 CameraScaleFirst = 0x34B014;
    static constexpr UInt32 CameraScaleSecond = 0x34B018;
    static constexpr UInt32 CameraScaleThird = 0x34B020;
    static constexpr UInt32 CameraScaleFourth = 0x34B024;

    static constexpr UInt32 ProjectionSetup = 0x38DA28;
    static constexpr UInt32 ProjectionDepth = 0x5F5CD0;

    static constexpr UInt32 MovieStart = 0x39C7D8;
    static constexpr UInt32 MovieStartResume = 0x39C7DC;

    static constexpr UInt32 MovieSkipPoll = 0x39C9D4;
    static constexpr UInt32 MovieSkipLoop = 0x39CCE8;

    static constexpr UInt32 DmaStart = 0x343028;
    static constexpr UInt32 DmaStartResume = 0x343030;
    static constexpr UInt32 GifChannel = 0x1000A000;
    static constexpr UInt32 Vif1Channel = 0x10009000;

    static constexpr UInt32 FrameCopyLoops = 280;
    static constexpr UInt32 FrameCopyOriginX = 0x6C00;
    static constexpr UInt32 FrameCopyOriginY = 0x7900;
    static constexpr UInt32 FrameCopyWidth = 640;
    static constexpr UInt32 FrameCopyHeight = 224;

    static constexpr UInt32 HudFirstCommit = 0x39958C;
    static constexpr UInt32 HudFirstCommitResume = 0x399594;
    static constexpr UInt32 HudSecondCommit = 0x39965C;
    static constexpr UInt32 HudSecondCommitResume = 0x399664;
    static constexpr UInt32 HudCommitBase = 0x8C;
    static constexpr UInt32 HudBufferState = 0x8BD310;
    static constexpr UInt32 HudFirstBufferPointer = 0x40;
    static constexpr UInt32 HudSecondBufferPointer = 0x48;
    static constexpr UInt32 HudFirstBufferIndex = 0x50;
    static constexpr UInt32 HudSecondBufferIndex = 0x54;
    static constexpr UInt32 HudFirstBufferBase = 0x5F5E14;
    static constexpr UInt32 HudSecondBufferBase = 0x5F5E18;
    static constexpr UInt32 HudFirstBufferShift = 15;
    static constexpr UInt32 HudSecondBufferStride = 0x30000;
    static constexpr UInt32 HudFirstBufferSize = 0x7000;
    static constexpr UInt32 HudSecondBufferSize = 0x2C000;
};
