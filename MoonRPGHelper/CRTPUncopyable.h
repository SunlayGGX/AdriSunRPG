#pragma once

namespace MoonRPG
{
    template<class Type>
    struct Uncopyable
    {
    public:
        Uncopyable() = default;


    private:
        Uncopyable(const Uncopyable&) = delete;
        Uncopyable& operator=(const Uncopyable&) = delete;
    };
}