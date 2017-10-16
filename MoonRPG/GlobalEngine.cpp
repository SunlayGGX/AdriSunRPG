#include "GlobalEngine.h"

#include "RenderEngine.h"
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
    RenderEngine::instance().initialize();
}

void GlobalEngine::destroy()
{
    this->quit();

    RenderEngine::instance().destroy();
}

void GlobalEngine::run()
{
    LOG_INFO("Start Running Engine");
    while(m_run)
    {
        /*TODO : Update the Game Loop*/
        RenderEngine::instance().update();

        //Bad, but until we have a proper TimeManager, this will do the job...
        //TODO : Call a proper method from a Time Manager or Synchronize with VSync from RenderEngine swap chain present (present1 method) or so
        std::this_thread::sleep_for(std::chrono::milliseconds{ 1000 / MoonRPG::ENGINE_FPS }); 
    }
    LOG_INFO("Engine stop running");
}

void GlobalEngine::quit()
{
    m_run = false;
}
