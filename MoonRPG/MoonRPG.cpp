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
    //TODO Add path for save file
    MoonRPG::LoggerManager::instance().saveAllLogFiles();

    ::MessageBoxA(NULL, exception.what(), "Error", MB_ICONERROR);
    return -99;
}
catch(...)
{
    //TODO Add path for save file
    MoonRPG::LoggerManager::instance().saveAllLogFiles();

    ::MessageBoxA(NULL, "Unidentified error", "Error", MB_ICONERROR);
    return -100;
}
