// MoonRPG.cpp : Entry point
//

#include "AutoEngineInstaller.h"

#include "GlobalEngine.h"
#include "LoggerManager.h"


int main() try
{
    MoonRPG::AutoEngineInstaller installEngine;

    MoonRPG::GlobalEngine::instance().run();

    LOG_VS_ERROR("LOG TEST ERROR");
    LOG_VS_WARNING("LOG TEST WARNIGN");
    LOG_VS_CONFIG("LOG TEST CONFIG");
    LOG_VS_INFO("LOG TEST INFO");
    LOG_VS_TRACE("LOG TEST TRACE");
    LOG_VS_DEBUG("LOG TEST DEBUG");

    return 0;
}
catch(const std::exception& exception)
{
    //TODO Add path for save file
    MoonRPG::LoggerManager::instance().saveFileToSafeDirectory("");

    ::MessageBoxA(NULL, exception.what(), "Error", MB_ICONERROR);
    return -99;
}
catch(...)
{
    //TODO Add path for save file
    MoonRPG::LoggerManager::instance().saveFileToSafeDirectory("");

    ::MessageBoxA(NULL, "Unidentified error", "Error", MB_ICONERROR);
    return -100;
}
