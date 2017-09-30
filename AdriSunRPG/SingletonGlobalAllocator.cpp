#include "SingletonGlobalAllocator.h"

#include "GlobalEngine.h"
#include "WindowManager.h"
#include "RenderEngine.h"


namespace AdriSunRPG
{
    using SingletonAllocatorAlias = AdriSunSingletonGlobalAllocator<
        GlobalEngine,
        WindowManager,
        RenderEngine
    >;
}


AdriSunRPG::SingletonAllocatorAlias g_singletonSimpleStackAllocator;
