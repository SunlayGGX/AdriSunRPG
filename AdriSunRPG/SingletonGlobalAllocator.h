#pragma once

#include "SizeCounter.h"


namespace AdriSunRPG
{
    template<class SingletonType>
    class AdriSunSingletonAllocatorHelper
    {
        static_assert(
            sizeof(SingletonType) != 0,
            ADRISUN_NORMALIZE_STATIC_ASSERT_MESSAGE_OUTSIDE_FUNCTION("The singleton object must be correctly declared (no prototype). Have you forgotten an include somewhere?")
            );


    public:
        static void allocate(char* &where)
        {
            SingletonType::s_instance = new(static_cast<void*>(where)) SlgSingleton;
            where += sizeof(SingletonType);
        }

        static void deallocate()
        {
            SingletonType::s_instance->~SingletonType();
        }
    };


    template<class ... Args>
    class alignas(SYSTEM_CACHE_ALIGNMENT_SIZE) AdriSunSingletonGlobalAllocator
    {
    public:
        enum
        {
            MEMORY_SIZE_IN_BYTES = AdriSunRPG::SizeCounter<Args...>::value
        };

        static_assert(sizeof...(Args) > 0, ADRISUN_NORMALIZE_STATIC_ASSERT_MESSAGE_OUTSIDE_FUNCTION("The argument count must be superior or equal to 1"));
        static_assert(AdriSunRPG::AdriSunSingletonGlobalAllocator<Args...>::MEMORY_SIZE_IN_BYTES > 0, ADRISUN_NORMALIZE_STATIC_ASSERT_MESSAGE_OUTSIDE_FUNCTION("The memory size allocated must be superior to 0"));


    private:
        char m_bufferMemory[AdriSunRPG::AdriSunSingletonGlobalAllocator<Args...>::MEMORY_SIZE_IN_BYTES];


    private:
        template<class SlgSingleton, class ... RemainingSingletons>
        class AdriSunSingletonGlobalAllocatorImpl
        {
        public:
            static void allocate(char* &where)
            {
                AdriSunSingletonGlobalAllocatorImpl<SlgSingleton>::allocate(where);
                AdriSunSingletonGlobalAllocatorImpl<RemainingSingletons...>::allocate(where);
            }

            static void deallocate()
            {
                AdriSunSingletonGlobalAllocatorImpl<RemainingSingletons...>::deallocate();
                AdriSunSingletonGlobalAllocatorImpl<SlgSingleton>::deallocate();
            }
        };

        template<class SlgSingleton>
        class AdriSunSingletonGlobalAllocatorImpl<SlgSingleton>
        {
        public:
            static void allocate(char* &where)
            {
                AdriSunSingletonAllocatorHelper<SlgSingleton>::allocate(where);
            }

            static void deallocate()
            {
                AdriSunSingletonAllocatorHelper<SlgSingleton>::deallocate();
            }
        };


    public:
        AdriSunSingletonGlobalAllocator()
        {
            char* begin = m_bufferMemory;
            AdriSunSingletonGlobalAllocatorImpl<Args...>::allocate(begin);
        }

        ~AdriSunSingletonGlobalAllocator()
        {
            AdriSunSingletonGlobalAllocatorImpl<Args...>::deallocate();
        }
    };
}

