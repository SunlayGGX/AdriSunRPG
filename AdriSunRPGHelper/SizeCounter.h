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
            value = sizeof(FirstType) + SizeCounter<OtherType...>::value,

            cache_optimized_value = SizeCounter<FirstType>::cache_optimized_value + SizeCounter<OtherType...>::cache_optimized_value
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
            value = sizeof(LonelyType),

            remainingOnCacheLine = value % SYSTEM_CACHE_ALIGNMENT_SIZE,
            isCacheLineEqual = remainingOnCacheLine == 0,
            toAddToCompleteCacheLine = isCacheLineEqual ? 0 : SYSTEM_CACHE_ALIGNMENT_SIZE - remainingOnCacheLine,

            cache_optimized_value = value + toAddToCompleteCacheLine
        };
    };
}
