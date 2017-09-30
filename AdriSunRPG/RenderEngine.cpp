#include "RenderEngine.h"

#include "GameConfig.h"

#include "GlobalEngine.h"
#include "WindowManager.h"

#include <thread>
#include <mutex>

using namespace AdriSunRPG;


RenderEngine::RenderEngine() :
    m_windowsHandleInstance{ nullptr }
{
    
}

RenderEngine::~RenderEngine()
{
}

void RenderEngine::initialize()
{
    std::thread
    {
        [&synchronizer = this->m_synchronizer, &windowMgr = WindowManager::instance()]() {
            windowMgr.initialize();

            synchronizer.notify_all();

            while(GlobalEngine::instance().m_run)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds{ AdriSunRPG::WINDOWS_THREAD_UPDATE_RATE_IN_MILLISECONDS });
                windowMgr.update();
            }

            windowMgr.destroy();
        }
    }.detach();

    m_synchronizer.wait(std::unique_lock<std::mutex>{ m_mutex });

    if(GlobalEngine::instance().m_run)
    {
        assert(m_windowsHandleInstance != nullptr);
        
        m_globalDevice.initialize(m_windowsHandleInstance);
    }
}

void RenderEngine::destroy()
{
    m_synchronizer.notify_all();
}

void RenderEngine::update()
{
    m_globalDevice.initView();

    m_globalDevice.clearView();

    /*
    TODO : Here, draw everything on scene
    */

    m_globalDevice.unbindTargetView();

    m_globalDevice.presentToDisplay();
}

void RenderEngine::setWindowsHandleInstance(HWND windowInstance)
{
    std::lock_guard<std::mutex> autoLocker{ m_mutex };
    m_windowsHandleInstance = windowInstance;
}

HWND RenderEngine::getWindowInstance() const
{
    std::lock_guard<std::mutex> autoLocker{ m_mutex };
    return m_windowsHandleInstance;
}
