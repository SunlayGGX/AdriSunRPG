#pragma once

#include "SizeCounter.h"

#define MOON_ALLOCATE_SINGLETON_ON_CACHE_ALIGNMENT


#ifdef MOON_ALLOCATE_SINGLETON_ON_CACHE_ALIGNMENT
#   define MOON_ALIGN_SINGLETON_GLOBAL_ALLOCATOR alignas(SYSTEM_CACHE_ALIGNMENT_SIZE)
#   define MOON_RETRIEVE_ALLOCATED_SIZE_OF(Singletons) SizeCounter<Singletons>::cache_optimized_value
#else
#   define MOON_ALIGN_SINGLETON_GLOBAL_ALLOCATOR 
#   define MOON_RETRIEVE_ALLOCATED_SIZE_OF(Singletons) SizeCounter<Singletons>::value
#endif

namespace MoonRPG
{
    template<class SingletonType>
    class MoonSingletonAllocatorHelper
    {
        static_assert(
            sizeof(SingletonType) != 0,
            MOON_NORMALIZE_STATIC_ASSERT_MESSAGE_OUTSIDE_FUNCTION("The singleton object must be correctly declared (no prototype). Have you forgotten an include somewhere?")
            );


    public:
        static void allocate(char* &where)
        {
            SingletonType::s_instance = new(static_cast<void*>(where)) SingletonType;
            where += MOON_RETRIEVE_ALLOCATED_SIZE_OF(SingletonType);
        }

        static void deallocate()
        {
            SingletonType::s_instance->~SingletonType();
        }
    };


    template<class ... Args>
    class MOON_ALIGN_SINGLETON_GLOBAL_ALLOCATOR MoonSingletonGlobalAllocator
    {
    public:
        enum
        {
            MEMORY_SIZE_IN_BYTES = MOON_RETRIEVE_ALLOCATED_SIZE_OF(Args...)
        };


    public:
        static_assert(
            sizeof...(Args) > 0, 
            MOON_NORMALIZE_STATIC_ASSERT_MESSAGE_OUTSIDE_FUNCTION("The argument count must be superior or equal to 1")
            );

        static_assert(
            MoonRPG::MoonSingletonGlobalAllocator<Args...>::MEMORY_SIZE_IN_BYTES > 0, 
            MOON_NORMALIZE_STATIC_ASSERT_MESSAGE_OUTSIDE_FUNCTION("The memory size allocated must be superior to 0")
            );

#ifdef MOON_ALLOCATE_SINGLETON_ON_CACHE_ALIGNMENT
        static_assert(
            ((MoonRPG::MoonSingletonGlobalAllocator<Args...>::MEMORY_SIZE_IN_BYTES % SYSTEM_CACHE_ALIGNMENT_SIZE) == 0) &&
            (SizeCounter<Args...>::value <= MoonSingletonGlobalAllocator<Args...>::MEMORY_SIZE_IN_BYTES),
            MOON_NORMALIZE_STATIC_ASSERT_MESSAGE_OUTSIDE_FUNCTION("Error in cach optimized algorithm. Comment the macro MOON_ALLOCATE_SINGLETON_ON_CACHE_ALIGNMENT and give a feed back at sunlay.g@free.fr. Tell me that you found a case where it doesn't work. Thanks a lot ^^")
            );
#endif


    private:
        char m_bufferMemory[MoonRPG::MoonSingletonGlobalAllocator<Args...>::MEMORY_SIZE_IN_BYTES];


    private:
        template<class SlgSingleton, class ... RemainingSingletons>
        class MoonSingletonGlobalAllocatorImpl
        {
        public:
            static void allocate(char* &where)
            {
                MoonSingletonGlobalAllocatorImpl<SlgSingleton>::allocate(where);
                MoonSingletonGlobalAllocatorImpl<RemainingSingletons...>::allocate(where);
            }

            static void deallocate()
            {
                MoonSingletonGlobalAllocatorImpl<RemainingSingletons...>::deallocate();
                MoonSingletonGlobalAllocatorImpl<SlgSingleton>::deallocate();
            }
        };

        template<class SlgSingleton>
        class MoonSingletonGlobalAllocatorImpl<SlgSingleton>
        {
        public:
            static void allocate(char* &where)
            {
                MoonSingletonAllocatorHelper<SlgSingleton>::allocate(where);
            }

            static void deallocate()
            {
                MoonSingletonAllocatorHelper<SlgSingleton>::deallocate();
            }
        };


    public:
        MoonSingletonGlobalAllocator()
        {
            char* begin = m_bufferMemory;
            MoonSingletonGlobalAllocatorImpl<Args...>::allocate(begin);

            assert(begin == (m_bufferMemory + MoonRPG::MoonSingletonGlobalAllocator<Args...>::MEMORY_SIZE_IN_BYTES));
        }

        ~MoonSingletonGlobalAllocator()
        {
            MoonSingletonGlobalAllocatorImpl<Args...>::deallocate();
        }
    };
}

