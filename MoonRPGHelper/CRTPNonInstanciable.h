#pragma once

#pragma warning(disable:4624)

namespace MoonRPG
{
    template<class Type>
    struct NonInstanciable
    {
    private:
        template<class ... Args>
        NonInstanciable(Args& ...) = delete;
        ~NonInstanciable() = delete;
    };
}