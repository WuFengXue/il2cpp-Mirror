#include "Include/C/Baselib_Memory.h"
#include "Include/C/Baselib_Timer.h"
#include "Source/Baselib_Utilities.h"

#include "Source/Posix/Baselib_SystemSemaphore_PosixApi.inl.h"

#include <algorithm>

namespace Emscripten_WithPThreads
{
    using PosixApi::Baselib_SystemSemaphore_Create;
    using PosixApi::Baselib_SystemSemaphore_Acquire;
    using PosixApi::Baselib_SystemSemaphore_TryAcquire;
    using PosixApi::Baselib_SystemSemaphore_TryTimedAcquire;
    using PosixApi::Baselib_SystemSemaphore_Release;
    using PosixApi::Baselib_SystemSemaphore_Free;
}

namespace Emscripten_NoThreads
{
namespace detail
{
    struct Semaphore
    {
        uint32_t counter;
    };
}

    BASELIB_INLINE_IMPL Baselib_SystemSemaphore_Handle Baselib_SystemSemaphore_Create()
    {
        detail::Semaphore* semaphore = static_cast<detail::Semaphore*>(Baselib_Memory_Allocate(sizeof(detail::Semaphore)));
        semaphore->counter = 0;
        return ::detail::AsBaselibHandle<Baselib_SystemSemaphore_Handle>(semaphore);
    }

    BASELIB_INLINE_IMPL void Baselib_SystemSemaphore_Acquire(Baselib_SystemSemaphore_Handle _semaphore)
    {
        while (!Baselib_SystemSemaphore_TryTimedAcquire(_semaphore, 1000))
        {
            detail::Semaphore* semaphore = ::detail::AsNativeType<detail::Semaphore*>(_semaphore);
            BaselibAssert(semaphore->counter != 0);
        }
    }

    BASELIB_INLINE_IMPL bool Baselib_SystemSemaphore_TryAcquire(Baselib_SystemSemaphore_Handle _semaphore)
    {
        detail::Semaphore* semaphore = ::detail::AsNativeType<detail::Semaphore*>(_semaphore);
        if (semaphore->counter == 0)
            return false;
        else
        {
            semaphore->counter -= 1;
            return true;
        }
    }

    BASELIB_INLINE_IMPL bool Baselib_SystemSemaphore_TryTimedAcquire(Baselib_SystemSemaphore_Handle _semaphore, uint32_t timeoutInMilliseconds)
    {
        detail::Semaphore* semaphore = ::detail::AsNativeType<detail::Semaphore*>(_semaphore);
        if (semaphore->counter == 0)
        {
            Baselib_Timer_WaitForAtLeast(timeoutInMilliseconds);
            return false;
        }
        else
        {
            semaphore->counter -= 1;
            return true;
        }
    }

    BASELIB_INLINE_IMPL void Baselib_SystemSemaphore_Release(Baselib_SystemSemaphore_Handle _semaphore, uint32_t count)
    {
        detail::Semaphore* semaphore = ::detail::AsNativeType<detail::Semaphore*>(_semaphore);
        semaphore->counter = std::min<uint64_t>(semaphore->counter + count, Baselib_SystemSemaphore_MaxCount);
    }

    BASELIB_INLINE_IMPL void Baselib_SystemSemaphore_Free(Baselib_SystemSemaphore_Handle _semaphore)
    {
        Baselib_Memory_Free(::detail::AsNativeType<detail::Semaphore*>(_semaphore));
    }
}
#include "Include/C/Baselib_SystemSemaphore.h"
