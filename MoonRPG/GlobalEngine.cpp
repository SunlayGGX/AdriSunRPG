#include "GlobalEngine.h"

#include "RenderEngine.h"

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
    constexpr const std::chrono::milliseconds updateTime{ 1000 / MoonRPG::ENGINE_FPS };

    while(m_run && !this->isFullyInitialized())
    {
        std::this_thread::sleep_for(updateTime);
    }

    while(m_run)
    {
        /*TODO : Update the Game Loop*/

        RenderEngine::instance().update();

        //Bad, but until we have a proper TimeManager, this will do the job...
        //TODO : Call a proper method from a Time Manager or Synchronize with VSync from RenderEngine swap chain present (present1 method) or so
        std::this_thread::sleep_for(updateTime);
    }
}

void GlobalEngine::quit()
{
    m_run = false;

bool GlobalEngine::isFullyInitialized() const
{
    return 
        RenderEngine::instance().isInitialized();
}