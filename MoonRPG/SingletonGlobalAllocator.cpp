#include "SingletonGlobalAllocator.h"

#include "GlobalEngine.h"
#include "WindowManager.h"
#include "RenderEngine.h"


namespace MoonRPG
{
    using SingletonAllocatorAlias = MoonSingletonGlobalAllocator<
        GlobalEngine,
        WindowManager,
        RenderEngine
    >;
}


MoonRPG::SingletonAllocatorAlias g_singletonSimpleStackAllocator;
