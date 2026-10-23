#pragma once
#include "Types.h"

#include "game/IntroTrampoline.h"

class CIntro final {
public:
    static void Install();

private:
    static void OnMovieStart(UInt32 id);
    static void Apply(bool skip);

    alignas(16) static inline UInt32 s_trampoline[CIntroTrampoline::Words]{};
    static inline UInt32 s_originalPoll = 0;
    static inline UInt32 s_originalLoop = 0;
    static inline bool s_introSeen = false;
};
