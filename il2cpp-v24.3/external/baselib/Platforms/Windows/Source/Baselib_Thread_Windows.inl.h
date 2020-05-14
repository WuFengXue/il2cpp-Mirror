#include "Source/WinApi/Baselib_Thread_WinApi.inl.h"

namespace WinApi
{
namespace detail
{
    typedef HRESULT(WINAPI *SetThreadDescriptionFunc)(HANDLE, PCWSTR);

    static SetThreadDescriptionFunc GetSetThreadDescriptionFunc()
    {
        static SetThreadDescriptionFunc s_pSetThreadDescription =
            reinterpret_cast<SetThreadDescriptionFunc>(GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "SetThreadDescription"));
        return s_pSetThreadDescription;
    }

    BASELIB_INLINE_IMPL void Thread_SetName(Baselib_Thread* thread, const char* name)
    {
        static SetThreadDescriptionFunc s_pSetThreadDescription = GetSetThreadDescriptionFunc();
        if (s_pSetThreadDescription)
            s_pSetThreadDescription(thread->handle, WinApi_StringConversions_UTF8ToUTF16(name).c_str());
        else
        {
            WinApi::Thread_SetName_ViaException(GetThreadId(thread->handle), name);
        }
    }
}
}
