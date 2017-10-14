#include "AutoEngineInstaller.h"

#include "GlobalEngine.h"
#include "LoggerManager.h"

using namespace MoonRPG;


AutoEngineInstaller::AutoEngineInstaller()
{
    MoonRPG::LoggerManager::instance().initialize();
    MoonRPG::LoggerManager::instance().enableLogInFile();

    LOG_CONFIG("Install GlobalEngine");
    GlobalEngine::instance().initialize();
}

AutoEngineInstaller::~AutoEngineInstaller()
{
    LOG_CONFIG("Destroy GlobalEngine");
    GlobalEngine::instance().destroy();

    MoonRPG::LoggerManager::instance().destroy();
}
