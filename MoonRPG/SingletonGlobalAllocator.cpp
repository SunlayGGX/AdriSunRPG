#include "SingletonGlobalAllocator.h"

#include "GlobalEngine.h"
#include "TimeManager.h"
#include "WindowManager.h"
#include "RenderEngine.h"


namespace MoonRPG
{
    using SingletonAllocatorAlias = MoonSingletonGlobalAllocator<
        GlobalEngine,
        TimeManager,
        WindowManager,
        RenderEngine
    >;
}


MoonRPG::SingletonAllocatorAlias g_singletonSimpleStackAllocator;
