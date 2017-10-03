#pragma once

#include "UtilsMacro.h"
#include "CRTPUncopyable.h"


namespace MoonRPG
{
    MOON_DECLARE_SLG_SINGLETON_ALLOCATOR_HELPER;

    template<class Type>
    class MoonSingleton : private MoonRPG::Uncopyable<MoonSingleton<Type>>
    {
    private:
        friend Type;
        friend MoonSingletonAllocatorHelper<Type>;


    private:
        static Type* s_instance;


    private:
        MoonSingleton() = default;
        virtual ~MoonSingleton() = default;


    public:
        static Type& instance()
        {
            return *MoonSingleton<Type>::s_instance;
        }

        virtual void initialize() = 0;
        virtual void destroy() = 0;
    };


    template<class Type> Type* MoonSingleton<Type>::s_instance = nullptr;
}


#define GENERATE_CODE_FROM_MoonSingleton(Type)               \
private:                                                    \
    friend MoonSingleton<Type>;                              \
    friend MoonSingletonAllocatorHelper<Type>;               \
private:                                                    \
    Type();                                                 \
    ~Type();                                                \
public:                                                     \
    using MoonSingleton<Type>::instance
