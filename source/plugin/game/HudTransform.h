#pragma once
#include "Types.h"

#include <string.h>

class CHudTransform final {
public:
    struct CSettings {
        Int32 scale;
        bool pillarboxTextured;
    };

    static UInt8* TransformRecords(UInt8* cursor, UInt8* end, const UInt8* limit, const CSettings& settings) {
        UInt8* append = end;

        while (cursor < end) {
            const UInt64 tag = Load64(cursor);
            const UInt64 registers = Load64(cursor + 8);
            const UInt32 loops = static_cast<UInt32>(tag & 0x7FFF);
            const UInt32 format = static_cast<UInt32>((tag >> 58) & 3);
            UInt32 count = static_cast<UInt32>(tag >> 60);
            if (count == 0)
                count = 16;

            if (format == RegisterList) {
                const bool backdrop = loops == 1 && IsBackdropShape(count, registers);
                UInt8 original[SpriteBytes];
                if (backdrop)
                    memcpy(original, cursor, SpriteBytes);

                TransformEntries(cursor + 16, tag, RegisterList, loops, count, registers, settings);

                if (backdrop)
                    append = AppendStrips(original, cursor, append, limit);

                cursor += 16 + ((loops * count * RegisterListStride + 15) & ~15u);
            }
            else if (format == Packed) {
                TransformEntries(cursor + 16, tag, Packed, loops, count, registers, settings);
                cursor += 16 + loops * count * 16;
            }
            else {
                return append;
            }
        }
        return append;
    }

private:
    static constexpr UInt32 Packed = 0;
    static constexpr UInt32 RegisterList = 1;
    static constexpr UInt32 PackedStride = 16;
    static constexpr UInt32 RegisterListStride = 8;
    static constexpr UInt32 AddressData = 0xE;
    static constexpr UInt32 PrimitiveRegister = 0;
    static constexpr UInt64 PrimitiveTextured = 0x10;
    static constexpr UInt64 PrimitiveGouraud = 0x8;
    static constexpr UInt32 MaxGradientHeight = 0x400;
    static constexpr UInt64 TagPrimitiveEnable = 1ULL << 46;
    static constexpr UInt32 TagPrimitiveShift = 47;
    static constexpr Int32 ScreenCenter = 0x8000;
    static constexpr UInt32 FullWidth = 0x2580;
    static constexpr UInt32 FirstOnScreen = 0x2000;
    static constexpr UInt32 LastOnScreen = 0xE000;

    static constexpr UInt32 SpriteRegisterCount = 7;
    static constexpr UInt64 SpriteRegisters = 0x5353170;
    static constexpr UInt32 SpriteBytes = 80;
    static constexpr UInt32 PrimitiveEntry = 16;
    static constexpr UInt32 ColorEntry = 16 + 8 * 2;
    static constexpr UInt32 FirstVertex = 16 + 8 * 4;
    static constexpr UInt32 SecondVertex = 16 + 8 * 6;
    static constexpr UInt64 BlackOpaque = 0x80000000ULL;
    static constexpr UInt64 ColorKeepMask = 0xFFFFFFFF00000000ULL;

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

    static void Store16(UInt8* address, UInt16 value) {
        memcpy(address, &value, sizeof(value));
    }

    static void Store64(UInt8* address, UInt64 value) {
        memcpy(address, &value, sizeof(value));
    }

    static bool IsVertexRegister(UInt32 registerId) {
        return ((registerId | 8u) & 0xEu) == 0xCu;
    }

    static bool IsBackdropShape(UInt32 count, UInt64 registers) {
        return count == SpriteRegisterCount && registers == SpriteRegisters;
    }

    static bool IsOnScreen(UInt32 x) {
        return x >= FirstOnScreen && x < LastOnScreen;
    }

    static UInt8* VertexOf(UInt8* payload, UInt32 format, UInt32 entry, UInt32 registerId) {
        if (format == RegisterList)
            return IsVertexRegister(registerId) ? payload + RegisterListStride * entry : nullptr;

        UInt8* qword = payload + PackedStride * entry;
        if (IsVertexRegister(registerId))
            return qword;

        if (registerId == AddressData && qword[8] < 0x10 && IsVertexRegister(qword[8]))
            return qword;
        
        return nullptr;
    }

    static bool IsPillarboxed(UInt64 primitive, UInt32 height) {
        if (primitive & PrimitiveTextured)
            return true;
        const UInt64 type = primitive & 7;
        return (primitive & PrimitiveGouraud) && type >= 3 && type <= 5 && height <= MaxGradientHeight;
    }

    static bool IsPillarboxed(UInt8* payload, UInt64 tag, UInt32 format, UInt32 loops, UInt32 count, UInt64 registers, UInt32 height) {
        if (format == Packed && (tag & TagPrimitiveEnable) && IsPillarboxed((tag >> TagPrimitiveShift) & 0x7FF, height))
            return true;

        const UInt32 stride = format == Packed ? PackedStride : RegisterListStride;
        for (UInt32 entry = 0; entry < loops * count; entry++) {
            const UInt32 registerId = (registers >> (4 * (entry % count))) & 0xF;
            const UInt8* data = payload + stride * entry;
            if (registerId == PrimitiveRegister && IsPillarboxed(Load64(data) & 0x7FF, height))
                return true;
            if (format == Packed && registerId == AddressData && data[8] == 0 && IsPillarboxed(Load64(data) & 0x7FF, height))
                return true;
        }
        return false;
    }

    static void TransformEntries(UInt8* payload, UInt64 tag, UInt32 format, UInt32 loops, UInt32 count, UInt64 registers, const CSettings& settings) {
        UInt32 low = 0xFFFF;
        UInt32 high = 0;
        UInt32 top = 0xFFFF;
        UInt32 bottom = 0;
        bool any = false;

        for (UInt32 entry = 0; entry < loops * count; entry++) {
            const UInt8* vertex = VertexOf(payload, format, entry, (registers >> (4 * (entry % count))) & 0xF);
            if (!vertex)
                continue;

            const UInt32 x = Load16(vertex);
            if (!IsOnScreen(x))
                continue;

            if (x < low)
                low = x;

            if (x > high)
                high = x;

            const UInt32 y = Load16(vertex + 2);
            if (y < top)
                top = y;

            if (y > bottom)
                bottom = y;
            any = true;
        }

        if (!any)
            return;

        if (high - low >= FullWidth && !(settings.pillarboxTextured && IsPillarboxed(payload, tag, format, loops, count, registers, bottom - top)))
            return;

        for (UInt32 entry = 0; entry < loops * count; entry++) {
            UInt8* vertex = VertexOf(payload, format, entry, (registers >> (4 * (entry % count))) & 0xF);
            if (!vertex || !IsOnScreen(Load16(vertex)))
                continue;

            const Int32 offset = static_cast<Int32>(Load16(vertex)) - ScreenCenter;
            const Int32 scaled = static_cast<Int32>(static_cast<Int64>(offset) * settings.scale >> 16);
            Store16(vertex, static_cast<UInt16>(scaled + ScreenCenter));
        }
    }

    static void MakeBlack(UInt8* strip) {
        Store64(strip + PrimitiveEntry, Load64(strip + PrimitiveEntry) & ~PrimitiveTextured);
        Store64(strip + ColorEntry, (Load64(strip + ColorEntry) & ColorKeepMask) | BlackOpaque);
    }

    static UInt8* AppendStrips(const UInt8* original, const UInt8* moved, UInt8* append, const UInt8* limit) {
        const UInt16 leftOriginal = Load16(original + FirstVertex);
        const UInt16 rightOriginal = Load16(original + SecondVertex);
        const UInt16 leftMoved = Load16(moved + FirstVertex);
        const UInt16 rightMoved = Load16(moved + SecondVertex);

        if (rightOriginal < leftOriginal || static_cast<UInt32>(rightOriginal - leftOriginal) < FullWidth)
            return append;

        if (leftMoved <= leftOriginal || rightMoved >= rightOriginal)
            return append;

        if (append + 2 * SpriteBytes > limit)
            return append;

        UInt8* left = append;
        memcpy(left, moved, SpriteBytes);
        Store16(left + FirstVertex, leftOriginal);
        Store16(left + SecondVertex, leftMoved);
        MakeBlack(left);

        UInt8* right = append + SpriteBytes;
        memcpy(right, moved, SpriteBytes);
        Store16(right + FirstVertex, rightMoved);
        Store16(right + SecondVertex, rightOriginal);
        MakeBlack(right);

        return append + 2 * SpriteBytes;
    }
};
