#include "SingletonGlobalAllocator.h"

#include "GlobalEngine.h"
#include "WindowManager.h"
#include "RenderEngine.h"
#include "InputEngine.h"


namespace MoonRPG
{
    using SingletonAllocatorAlias = MoonSingletonGlobalAllocator<
        GlobalEngine,
        WindowManager,
        RenderEngine,
        InputEngine
    >;
}


MoonRPG::SingletonAllocatorAlias g_singletonSimpleStackAllocator;
