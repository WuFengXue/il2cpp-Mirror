namespace baselib
{
    BASELIB_CPP_INTERFACE
    {
        template<typename LockT>
        inline void ConditionVariable::Wait(LockT& lock)
        {
            m_Data.waiters.fetch_add(1, memory_order_relaxed);
            lock.Release();
            m_Data.semaphore.Acquire();
            lock.Acquire();
        }

        template<typename LockT>
        inline bool ConditionVariable::TimedWait(LockT& lock, const timeout_ms timeoutInMilliseconds)
        {
            m_Data.waiters.fetch_add(1, memory_order_relaxed);
            lock.Release();

            bool acquired = m_Data.semaphore.TryTimedAcquire(timeoutInMilliseconds);

            if (acquired)
            {
                lock.Acquire();
                return true;
            }

            do
            {
                uint32_t waiters = m_Data.waiters.load(memory_order_relaxed);
                while (waiters > 0)
                {
                    if (m_Data.waiters.compare_exchange_weak(waiters, waiters - 1, memory_order_relaxed, memory_order_relaxed))
                    {
                        lock.Acquire();
                        return false;
                    }
                }
                Baselib_Thread_YieldExecution();
            }
            while (!m_Data.semaphore.TryAcquire());

            lock.Acquire();
            return true;
        }

        inline void ConditionVariable::Notify(uint16_t count)
        {
            uint32_t waitingThreads, threadsToWakeup;
            do
            {
                waitingThreads = m_Data.waiters.load(memory_order_acquire);
                threadsToWakeup = count < waitingThreads ? count : waitingThreads;
                if (threadsToWakeup == 0)
                    return;
            }
            while (!m_Data.waiters.compare_exchange_weak(waitingThreads, waitingThreads - threadsToWakeup, memory_order_relaxed, memory_order_relaxed));
            m_Data.semaphore.Release(threadsToWakeup);
        }
    }
}
