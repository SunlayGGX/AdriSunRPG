#include "AutoEngineInstaller.h"

#include "GlobalEngine.h"

using namespace MoonRPG;


AutoEngineInstaller::AutoEngineInstaller()
{
    GlobalEngine::instance().initialize();
}

AutoEngineInstaller::~AutoEngineInstaller()
{
    GlobalEngine::instance().destroy();
}
