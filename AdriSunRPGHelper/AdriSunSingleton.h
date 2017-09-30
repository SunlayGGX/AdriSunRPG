#pragma once

#include "UtilsMacro.h"
#include "CRTPUncopyable.h"


namespace AdriSunRPG
{
    ADRISUN_DECLARE_SLG_SINGLETON_ALLOCATOR_HELPER;

    template<class Type>
    class AdriSunSingleton : virtual private AdriSunRPG::Uncopyable<AdriSunSingleton<Type>>
    {
    private:
        friend Type;
        friend AdriSunSingletonAllocatorHelper<Type>;


    private:
        static Type* s_instance;


    private:
        AdriSunSingleton() = default;
        virtual ~AdriSunSingleton() = default;


    public:
        static Type& instance()
        {
            return *AdriSunSingleton<Type>::s_instance;
        }

        virtual void initialize() = 0;
        virtual void destroy() = 0;
    };


    template<class Type> Type* AdriSunSingleton<Type>::s_instance = nullptr;
}


#define GENERATE_CODE_FROM_AdriSunSingleton(Type)               \
private:                                                    \
    friend AdriSunSingleton<Type>;                              \
    friend AdriSunSingletonAllocatorHelper<Type>;               \
private:                                                    \
    Type();                                                 \
    ~Type();                                                \
public:                                                     \
    using AdriSunSingleton<Type>::instance