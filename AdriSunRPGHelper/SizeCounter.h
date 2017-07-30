#pragma once

namespace AdriSunRPG
{
    /*
    Meta object to count the size of all elements.
    */
    template<class FirstType, class ... OtherType> 
    struct SizeCounter
    {
    public:
        enum
        {
            value = sizeof(FirstType) + SizeCounter<OtherType...>::value
        };
    };

    /*
    Version with one element
    */
    template<class LonelyType>
    struct SizeCounter<LonelyType>
    {
    public:
        enum
        {
            value = sizeof(LonelyType)
        };
    };
}
