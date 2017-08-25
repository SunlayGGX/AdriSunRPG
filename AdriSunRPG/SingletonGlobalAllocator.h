#pragma once

#include "SizeCounter.h"

#define ADRISUN_ALLOCATE_SINGLETON_ON_CACHE_ALIGNMENT


#ifdef ADRISUN_ALLOCATE_SINGLETON_ON_CACHE_ALIGNMENT
#   define ADRISUN_ALIGN_SINGLETON_GLOBAL_ALLOCATOR alignas(SYSTEM_CACHE_ALIGNMENT_SIZE)
#   define ADRISUN_RETRIEVE_ALLOCATED_SIZE_OF(Singletons) SizeCounter<Singletons>::cache_optimized_value
#else
#   define ADRISUN_ALIGN_SINGLETON_GLOBAL_ALLOCATOR 
#   define ADRISUN_RETRIEVE_ALLOCATED_SIZE_OF(Singletons) SizeCounter<Singletons>::value
#endif

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
            SingletonType::s_instance = new(static_cast<void*>(where)) SingletonType;
            where += ADRISUN_RETRIEVE_ALLOCATED_SIZE_OF(SingletonType);
        }

        static void deallocate()
        {
            SingletonType::s_instance->~SingletonType();
        }
    };


    template<class ... Args>
    class ADRISUN_ALIGN_SINGLETON_GLOBAL_ALLOCATOR AdriSunSingletonGlobalAllocator
    {
    public:
        enum
        {
            MEMORY_SIZE_IN_BYTES = ADRISUN_RETRIEVE_ALLOCATED_SIZE_OF(Args...)
        };


    public:
        static_assert(
            sizeof...(Args) > 0, 
            ADRISUN_NORMALIZE_STATIC_ASSERT_MESSAGE_OUTSIDE_FUNCTION("The argument count must be superior or equal to 1")
            );

        static_assert(
            AdriSunRPG::AdriSunSingletonGlobalAllocator<Args...>::MEMORY_SIZE_IN_BYTES > 0, 
            ADRISUN_NORMALIZE_STATIC_ASSERT_MESSAGE_OUTSIDE_FUNCTION("The memory size allocated must be superior to 0")
            );

#ifdef ADRISUN_ALLOCATE_SINGLETON_ON_CACHE_ALIGNMENT
        static_assert(
            ((AdriSunRPG::AdriSunSingletonGlobalAllocator<Args...>::MEMORY_SIZE_IN_BYTES % SYSTEM_CACHE_ALIGNMENT_SIZE) == 0) &&
            (SizeCounter<Args...>::value <= AdriSunSingletonGlobalAllocator<Args...>::MEMORY_SIZE_IN_BYTES),
            ADRISUN_NORMALIZE_STATIC_ASSERT_MESSAGE_OUTSIDE_FUNCTION("Error in cach optimized algorithm. Comment the macro ADRISUN_ALLOCATE_SINGLETON_ON_CACHE_ALIGNMENT and give a feed back at sunlay.g@free.fr. Tell me that you found a case where it doesn't work. Thanks a lot ^^")
            );
#endif


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

            assert(begin == (m_bufferMemory + AdriSunRPG::AdriSunSingletonGlobalAllocator<Args...>::MEMORY_SIZE_IN_BYTES));
        }

        ~AdriSunSingletonGlobalAllocator()
        {
            AdriSunSingletonGlobalAllocatorImpl<Args...>::deallocate();
        }
    };
}

