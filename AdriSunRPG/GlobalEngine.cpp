#include "GlobalEngine.h"

#include "GameConfig.h"

using namespace AdriSunRPG;


GlobalEngine::GlobalEngine() :
    m_run{ true }
{

}

GlobalEngine::~GlobalEngine()
{

}

void GlobalEngine::initialize()
{

}

void GlobalEngine::destroy()
{
    this->quit();

}

void GlobalEngine::run()
{
    while(m_run)
    {
        /*TODO : Update the Game Loop*/

        //Bad, but until we have a proper TimeManager, this will do the job...
        //TODO : Call a proper method from a Time Manager or Synchronize with VSync from RenderEngine swap chain present (present1 method) or so
        std::this_thread::sleep_for(std::chrono::milliseconds{ 1000 / AdriSunRPG::ENGINE_FPS }); 
    }
}

void GlobalEngine::quit()
{
    m_run = false;
}
