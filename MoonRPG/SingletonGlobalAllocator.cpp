#include "SingletonGlobalAllocator.h"

#include "GlobalEngine.h"
#include "TimeManager.h"
#include "WindowManager.h"
#include "RenderEngine.h"
#include "LoggerManager.h"


namespace MoonRPG
{
    using SingletonAllocatorAlias = MoonSingletonGlobalAllocator<
        LoggerManager,
        GlobalEngine,
        TimeManager,
        WindowManager,
        RenderEngine
    >;
}


MoonRPG::SingletonAllocatorAlias g_singletonSimpleStackAllocator;
