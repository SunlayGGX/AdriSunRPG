// MoonRPG.cpp : Entry point
//

#include "AutoEngineInstaller.h"

#include "GlobalEngine.h"
#include "ElephantLogger.h"


int main() try
{
    MoonRPG::AutoEngineInstaller installEngine;

    MoonRPG::GlobalEngine::instance().run();

    return 0;
}
catch(const std::exception& exception)
{
    MoonRPG::LoggerManager::instance().saveAllLogFiles();

    ::MessageBoxA(NULL, exception.what(), "Error", MB_ICONERROR);
    return -99;
}
catch(...)
{
    MoonRPG::LoggerManager::instance().saveAllLogFiles();

    ::MessageBoxA(NULL, "Unidentified error", "Error", MB_ICONERROR);
    return -100;
}
