#include "GlobalEngine.h"

#include "RenderEngine.h"
#include "InputEngine.h"
#include "WindowManager.h"
#include "TimeManager.h"
#include "ElephantLogger.h"
#include "GameConfig.h"

#include <thread>

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

    this->startInputAndWindowsThread();
}

void GlobalEngine::destroy()
{
    this->quit();

    TimeManager::instance().destroy();

    RenderEngine::instance().destroy();
}

void GlobalEngine::run()
{
    constexpr const std::chrono::milliseconds updateTime{ 1000 / MoonRPG::ENGINE_FPS };

    while(m_run && !this->isFullyInitialized())
    {
        std::this_thread::sleep_for(updateTime);
    }

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

void GlobalEngine::startInputAndWindowsThread() const
{
    std::thread
    {
        [&run = this->m_run, &windowMgr = WindowManager::instance(), &inputMgr = InputEngine::instance()]() 
    {
        windowMgr.initialize();
        inputMgr.initialize();

        inputMgr.createKeyboard(windowMgr.getWindowHandle());

        while(run)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds{ 1000 / MoonRPG::ENGINE_FPS/*MoonRPG::WINDOWS_THREAD_UPDATE_RATE_IN_MILLISECONDS*/ });
            inputMgr.update();
            windowMgr.update();
        }

        inputMgr.destroy();
        windowMgr.destroy();
    }}.detach();
}

bool GlobalEngine::isFullyInitialized() const
{
    return 
        RenderEngine::instance().isInitialized();
}