#include "AutoEngineInstaller.h"

#include "GlobalEngine.h"

using namespace AdriSunRPG;


AutoEngineInstaller::AutoEngineInstaller()
{
    GlobalEngine::instance().initialize();
}

AutoEngineInstaller::~AutoEngineInstaller()
{
    GlobalEngine::instance().destroy();
}
