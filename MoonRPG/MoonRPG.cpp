// MoonRPG.cpp : Entry point
//

#include "AutoEngineInstaller.h"

#include "GlobalEngine.h"
#include "LoggerManager.h"


int main() try
{
    MoonRPG::LoggerManager::instance().initialize();

    LOG_CONFIG("Install Engine");
    MoonRPG::AutoEngineInstaller installEngine;

    LOG_INFO("Start Running Engine");
    MoonRPG::GlobalEngine::instance().run();

    return 0;
}
catch(const std::exception& exception)
{
    //TODO : LOG the error exception.what() and dump the application state.
    LOG_ERROR(exception.what()); // Add application state

    ::MessageBoxA(NULL, exception.what(), "Erreur", MB_ICONERROR);
    return -99;
}
catch(...)
{
    //TODO : LOG there was an error and dump the application state.
    ::MessageBoxA(NULL, "Unidentified error", "Erreur", MB_ICONERROR);
    return -100;
}
