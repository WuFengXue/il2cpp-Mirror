#pragma once

#include "Time.h"
#include <cstdint>

#if PLATFORM_FUTEX_NATIVE_SUPPORT
#include "Internal/ConditionVariableData_FutexBased.inl.h"
#else
#include "Internal/ConditionVariableData_SemaphoreBased.inl.h"
#endif

namespace baselib
{
    BASELIB_CPP_INTERFACE
    {
        /**
         * @brief Conceptually a condition variable is a queue of threads, associated with a monitor, on which a thread may wait for some condition to become true.
         *
         * Thus each condition variable c is associated with an assertion Pc. While a thread is waiting on a condition variable, that thread is not considered
         * to occupy the monitor, and so other threads may enter the monitor to change the monitor's state. In most types of monitors, these other threads may
         * signal the condition variable c to indicate that assertion Pc is true in the current state.
         *
         * "Monitor (synchronization)", Wikipedia: The Free Encyclopedia
         * https://en.wikipedia.org/w/index.php?title=Monitor_(synchronization)&oldid=914426020#Condition_variables_2
         *
         * For optimal performance, baselib::ConditionVariable should be stored at a cache aligned memory location.
         */
        class ConditionVariable
        {
        public:
            /// non-copyable
            ConditionVariable(const ConditionVariable& other) = delete;
            ConditionVariable& operator=(const ConditionVariable& other) = delete;

            /// non-movable (strictly speaking not needed but listed to signal intent)
            ConditionVariable(ConditionVariable&& other) = delete;
            ConditionVariable& operator=(ConditionVariable&& other) = delete;

            /**
             * @brief Creates a condition variable synchronization primitive.
             */
            ConditionVariable() = default;

            /**
             * @brief Reclaim resources and memory held by the condition variable.
             *
             * If threads are waiting on the condition variable, destructor will trigger an assert and may cause process abort.
             */
            ~ConditionVariable()
            {
                BaselibAssert(!m_Data.HasWaiters(), "Destruction is not allowed when there are still threads waiting on the condition variable.");
                NotifyAll();
            }

            /**
             * @brief Wait for the condition variable to become available.
             *
             * This function is guaranteed to emit an acquire barrier.
             *
             * @tparam LockT The type of the lock. Needs to provide `Acquire()` and `Release()`.
             * @param lock The lock that protects the monitor.
             */
            template<typename LockT>
            inline void Wait(LockT& lock);

            /**
             * @brief Wait for the condition variable to become available.
             *
             * This function is guaranteed to emit an acquire barrier.
             *
             * TimedWait with a zero timeout is guaranteed to be a user space operation.
             *
             * Timeout passed to this function may be subject to system clock resolution.
             * If the system clock has a resolution of e.g. 16ms that means this function may exit with a timeout error 16ms earlier than originally scheduled.
             *
             * @tparam LockT The type of the lock. Needs to provide `Acquire()` and `Release()`.
             * @param lock                    The lock that protects the monitor.
             * @param timeoutInMilliseconds   Time to wait for condition variable to become available.
             * @return true                   if the condition variable is available.
             * @return false                  if timeout was reached.
             */
            template<typename LockT>
            inline bool TimedWait(LockT& lock, const timeout_ms timeoutInMilliseconds);

            /**
             * @brief  Wake up threads waiting for the condition variable.
             *
             * This function is guaranteed to emit a release barrier.
             *
             * @param count At most, `count` waiting threads will be notified, but never more than there are currently waiting.
             */
            inline void Notify(uint16_t count);

            /**
             * @brief Wake up all threads waiting for the condition variable.
             *
             * This function is guaranteed to emit a release barrier.
             */
            inline void NotifyAll()
            {
                Notify(std::numeric_limits<uint16_t>::max());
            }

        private:
            detail::ConditionVariableData m_Data;
        };
    }
}

#if PLATFORM_FUTEX_NATIVE_SUPPORT
#include "Internal/ConditionVariable_FutexBased.inl.h"
#else
#include "Internal/ConditionVariable_SemaphoreBased.inl.h"
#endif
