#include "Source/WinApi/Baselib_Thread_WinApi.inl.h"

namespace WinApi
{
namespace detail
{
    static inline void Thread_SetName(Baselib_Thread* thread, const char* name)
    {
        WinApi::Thread_SetName_ViaException(GetThreadId(thread->handle), name);
    }
}
}
