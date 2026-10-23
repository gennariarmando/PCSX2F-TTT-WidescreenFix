#pragma once
#include "Types.h"

#include "game/TekkenAddresses.h"

#include <string.h>

class CFrameCopyTransform final {
public:
    static constexpr UInt32 SpriteLoopBytes = 80;
    static constexpr UInt32 BlobBytes = 16 + CTekkenAddresses::FrameCopyLoops * SpriteLoopBytes;
    static constexpr UInt32 BarBytes = 112;
    static constexpr UInt32 TagBytes = 16;

    static UInt32 ChainId(const UInt8* tag) {
        return static_cast<UInt32>(Load64(tag) >> 28) & 7;
    }

    static UInt32 ChainQuadwords(const UInt8* tag) {
        return static_cast<UInt32>(Load64(tag) & 0xFFFF);
    }

    static UInt32 ChainAddress(const UInt8* tag) {
        return static_cast<UInt32>(Load64(tag) >> 32) & 0x7FFFFFFF;
    }

    static UInt64 ChainTag(UInt32 quadwords, UInt32 id, UInt32 address) {
        return quadwords | (static_cast<UInt64>(id) << 28) | (static_cast<UInt64>(address) << 32);
    }

    static bool IsChain(const UInt8* tags) {
        return ChainId(tags) == IdRef && ChainQuadwords(tags) == StateQuadwords &&
            ChainId(tags + TagBytes) == IdRef && ChainQuadwords(tags + TagBytes) == TargetQuadwords &&
            ChainId(tags + 2 * TagBytes) == IdEnd && ChainQuadwords(tags + 2 * TagBytes) == BlobBytes / 16;
    }

    static bool IsBlob(const UInt8* blob) {
        const UInt64 tag = Load64(blob);
        return (tag & 0x7FFF) == CTekkenAddresses::FrameCopyLoops && ((tag >> 58) & 3) == 1 && (tag >> 60) == 10 &&
            Load64(blob + 8) == BlobRegisters;
    }

    static void Squeeze(UInt8* blob, Int32 scale) {
        for (UInt32 loop = 0; loop < CTekkenAddresses::FrameCopyLoops; loop++) {
            UInt8* sprites = blob + 16 + SpriteLoopBytes * loop;
            for (UInt32 i = 0; i < VertexEntries; i++)
                Store16(sprites + 8 * VertexEntry[i], Scaled(Load16(sprites + 8 * VertexEntry[i]), scale));
        }
    }

    static void BuildBars(UInt8* out, Int32 scale) {
        const UInt32 left = CTekkenAddresses::FrameCopyOriginX;
        const UInt32 right = left + CTekkenAddresses::FrameCopyWidth * 16;
        const UInt32 top = CTekkenAddresses::FrameCopyOriginY;
        const UInt32 bottom = top + CTekkenAddresses::FrameCopyHeight * 16;
        const UInt32 insideLeft = Scaled(left, scale);
        const UInt32 insideRight = Scaled(right, scale);

        Store64(out, 2 | (1ULL << 15) | (1ULL << 46) | (BarPrimitive << 47) | (3ULL << 60));
        Store64(out + 8, BarRegisters);

        UInt8* entry = out + 16;
        AppendBar(entry, left, top, insideLeft, bottom);
        AppendBar(entry + 48, insideRight, top, right, bottom);
    }

    static void BuildChain(UInt8* out, const UInt8* tags, UInt32 blobAddress, UInt32 barAddress) {
        memcpy(out, tags, 2 * TagBytes);
        Store64(out + 2 * TagBytes, ChainTag(BlobBytes / 16, IdRef, blobAddress));
        Store64(out + 2 * TagBytes + 8, 0);
        Store64(out + 3 * TagBytes, ChainTag(BarBytes / 16, IdEnd, barAddress));
        Store64(out + 3 * TagBytes + 8, 0);
    }

private:
    static constexpr UInt32 IdRef = 3;
    static constexpr UInt32 IdEnd = 0;
    static constexpr UInt32 StateQuadwords = 8;
    static constexpr UInt32 TargetQuadwords = 5;
    static constexpr UInt64 BlobRegisters = 0x5353053530ULL;
    static constexpr UInt64 BarRegisters = 0x551;
    static constexpr UInt64 BarPrimitive = 0x206;
    static constexpr UInt64 BarAlpha = 0x80;
    static constexpr Int32 ScreenCenter = 0x8000;
    static constexpr UInt32 VertexEntries = 4;
    static constexpr UInt32 VertexEntry[VertexEntries] = { 2, 4, 7, 9 };

    static UInt64 Load64(const UInt8* address) {
        UInt64 value;
        memcpy(&value, address, sizeof(value));
        return value;
    }

    static UInt16 Load16(const UInt8* address) {
        UInt16 value;
        memcpy(&value, address, sizeof(value));
        return value;
    }

    static void Store64(UInt8* address, UInt64 value) {
        memcpy(address, &value, sizeof(value));
    }

    static void Store16(UInt8* address, UInt16 value) {
        memcpy(address, &value, sizeof(value));
    }

    static UInt16 Scaled(UInt32 x, Int32 scale) {
        const Int32 offset = static_cast<Int32>(x) - ScreenCenter;
        return static_cast<UInt16>(static_cast<Int32>(static_cast<Int64>(offset) * scale >> 16) + ScreenCenter);
    }

    static void AppendBar(UInt8* entry, UInt32 x0, UInt32 y0, UInt32 x1, UInt32 y1) {
        Store64(entry, 0);
        Store64(entry + 8, BarAlpha << 32);
        Store64(entry + 16, x0 | (static_cast<UInt64>(y0) << 32));
        Store64(entry + 24, 0);
        Store64(entry + 32, x1 | (static_cast<UInt64>(y1) << 32));
        Store64(entry + 40, 0);
    }
};
