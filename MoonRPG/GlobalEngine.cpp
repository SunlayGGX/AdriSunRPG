#include "GlobalEngine.h"

#include "RenderEngine.h"
#include "TimeManager.h"
#include "ElephantLogger.h"
#include "GameConfig.h"

using namespace MoonRPG;


GlobalEngine::GlobalEngine() :
    m_run{ true }
{

}

GlobalEngine::~GlobalEngine()
{

}

void GlobalEngine::initialize()
{
    TimeManager::instance().initialize();

    RenderEngine::instance().initialize();
}

void GlobalEngine::destroy()
{
    this->quit();

    TimeManager::instance().destroy();

    RenderEngine::instance().destroy();
}

void GlobalEngine::run()
{
    LOG_INFO("Start Running Engine");
    while(m_run)
    {
        /*TODO : Update the Game Loop*/
        RenderEngine::instance().update();

        TimeManager::instance().waitEndOfFrame();
    }
    LOG_INFO("Engine stop running");
}

void GlobalEngine::quit()
{
    m_run = false;
}
