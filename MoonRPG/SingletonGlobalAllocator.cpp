#include "SingletonGlobalAllocator.h"

#include "GlobalEngine.h"
#include "TimeManager.h"
#include "WindowManager.h"
#include "RenderEngine.h"
#include "InputEngine.h"
#include "LoggerManager.h"


namespace MoonRPG
{
    using SingletonAllocatorAlias = MoonSingletonGlobalAllocator<
        LoggerManager,
        GlobalEngine,
        TimeManager,
        WindowManager,
        RenderEngine,
        InputEngine
    >;
}


MoonRPG::SingletonAllocatorAlias g_singletonSimpleStackAllocator;
