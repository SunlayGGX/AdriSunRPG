#include "SingletonGlobalAllocator.h"

#include "GlobalEngine.h"
#include "WindowManager.h"
#include "RenderEngine.h"
#include "LoggerManager.h"


namespace MoonRPG
{
    using SingletonAllocatorAlias = MoonSingletonGlobalAllocator<
        LoggerManager,
        GlobalEngine,
        WindowManager,
        RenderEngine
    >;
}


MoonRPG::SingletonAllocatorAlias g_singletonSimpleStackAllocator;
