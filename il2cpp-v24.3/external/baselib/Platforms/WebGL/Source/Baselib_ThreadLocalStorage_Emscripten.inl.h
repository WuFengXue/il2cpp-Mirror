#include "Include/C/Baselib_ThreadLocalStorage.h"
#include "Include/C/Baselib_Memory.h"

#include "Source/AbortShim.h"

BASELIB_C_INTERFACE
{
    int32_t Baselib_Internal_Emscripten_TLS_SlotGenerations[PTHREAD_KEYS_MAX];
}

namespace Emscripten_WithPThreads
{
    BASELIB_INLINE_IMPL Baselib_TLS_Handle Baselib_TLS_Alloc(void)
    {
        pthread_key_t r;
        int rc = pthread_key_create(&r, [](void* tlsValue) { Baselib_Memory_Free(tlsValue); });
        if (rc != 0)
            ShimmableAbort(Baselib_ErrorCode_OutOfSystemResources);
        else
        {
            BaselibAssert(r < PTHREAD_KEYS_MAX);
            ++Baselib_Internal_Emscripten_TLS_SlotGenerations[r];
        }

        return static_cast<Baselib_TLS_Handle>(r);
    }

    BASELIB_INLINE_IMPL void Baselib_TLS_Free(Baselib_TLS_Handle handle)
    {
        int rc = pthread_key_delete(static_cast<pthread_key_t>(handle));
        BaselibAssert(rc == 0);
    }
}

namespace Emscripten_NoThreads
{
    BASELIB_INLINE_IMPL Baselib_TLS_Handle Baselib_TLS_Alloc(void)
    {
        uintptr_t* tlsAllocation = (uintptr_t*)Baselib_Memory_Allocate(sizeof(uintptr_t));
        *tlsAllocation = 0;
        return reinterpret_cast<Baselib_TLS_Handle>(tlsAllocation);
    }

    BASELIB_INLINE_IMPL void Baselib_TLS_Free(Baselib_TLS_Handle handle)
    {
        Baselib_Memory_Free(reinterpret_cast<void*>(handle));
    }
}
