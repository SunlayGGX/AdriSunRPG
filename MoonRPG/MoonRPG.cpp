// MoonRPG.cpp : Entry point
//

#include "AutoEngineInstaller.h"

#include "GlobalEngine.h"
#include "LoggerManager.h"


int main() try
{
    MoonRPG::LoggerManager::instance().initialize();
    MoonRPG::LoggerManager::instance().enableLogInFile();
    MoonRPG::LoggerManager::instance().run();

    MoonRPG::AutoEngineInstaller installEngine;

    MoonRPG::GlobalEngine::instance().run();

    return 0;
}
catch(const std::exception& exception)
{
    LOG_ERROR(exception.what()); // Add dump application state

    ::MessageBoxA(NULL, exception.what(), "Error", MB_ICONERROR);
    return -99;
}
catch(...)
{
    LOG_ERROR("Unknown error"); // Add dump application state
    ::MessageBoxA(NULL, "Unidentified error", "Error", MB_ICONERROR);
    return -100;
}
