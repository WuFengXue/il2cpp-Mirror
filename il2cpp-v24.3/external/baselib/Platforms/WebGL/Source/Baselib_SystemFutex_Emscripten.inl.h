#include "Include/C/Baselib_SystemFutex.h"
#include "Include/C/Baselib_Timer.h"
#include "Include/Cpp/Algorithm.h"
#include <emscripten/threading.h>

namespace Emscripten_WithPThreads
{
    BASELIB_INLINE_IMPL void Baselib_SystemFutex_Wait(int32_t* address, int32_t expected, uint32_t timeoutInMilliseconds)
    {
        emscripten_futex_wait(address, expected, (double)timeoutInMilliseconds);
    }

    BASELIB_INLINE_IMPL void Baselib_SystemFutex_Notify(int32_t* address, uint32_t count, Baselib_WakeupFallbackStrategy)
    {
        static_assert(sizeof(count) == sizeof(int32_t),
            "Emscripten_WithPThreads::Baselib_SystemFutex_Notify implementation relies on count being exactly 4 bytes");

        // Emscripten futex wait takes an integer, so we need to cap to its range.
        // In future emscripten version this is equivalent to wake all. See https://github.com/emscripten-core/emscripten/pull/8923
        if (count >= INT32_MAX)
            emscripten_futex_wake(address, INT32_MAX);
        else
            emscripten_futex_wake(address, count);
    }
}

namespace Emscripten_NoThreads
{
    BASELIB_INLINE_IMPL void Baselib_SystemFutex_Wait(int32_t* address, int32_t expected, uint32_t timeoutInMilliseconds)
    {
        if (*address == expected) Baselib_Timer_WaitForAtLeast(timeoutInMilliseconds);
    }

    BASELIB_INLINE_IMPL void Baselib_SystemFutex_Notify(int32_t* address, uint32_t count, Baselib_WakeupFallbackStrategy wakeupFallbackStrategy)
    {
    }
}
