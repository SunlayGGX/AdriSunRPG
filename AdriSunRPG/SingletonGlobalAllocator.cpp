#include "SingletonGlobalAllocator.h"

#include "GlobalEngine.h"

using namespace AdriSunRPG;


AdriSunSingletonGlobalAllocator<
    GlobalEngine
> g_singletonSimpleStackAllocator;
