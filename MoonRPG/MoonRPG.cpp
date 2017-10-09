// MoonRPG.cpp : Entry point
//

#include "AutoEngineInstaller.h"

#include "GlobalEngine.h"


int main() try
{
    MoonRPG::AutoEngineInstaller installEngine;

    MoonRPG::GlobalEngine::instance().run();

    return 0;
}
catch(const std::exception& exception)
{
    //TODO : LOG the error exception.what() and dump the application state.
    ::MessageBoxA(NULL, exception.what(), "Erreur", MB_ICONERROR);
    return -99;
}
catch(...)
{
    //TODO : LOG there was an error and dump the application state.
    ::MessageBoxA(NULL, "Unidentified error", "Erreur", MB_ICONERROR);
    return -100;
}
