// AdriSunRPG.cpp : Entry point
//

#include "AutoEngineInstaller.h"


int main() try
{
    AdriSunRPG::AutoEngineInstaller installEngine;

    return 0;
}
catch(const std::exception& exception)
{
    //TODO : LOG the error exception.what() and dump the application state.
    return -99;
}
catch(...)
{
    //TODO : LOG there was an error and dump the application state.
    return -100;
}
