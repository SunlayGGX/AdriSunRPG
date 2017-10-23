#include "RenderEngine.h"

#include "GameConfig.h"


using namespace MoonRPG;


RenderEngine::RenderEngine() :
    m_initialized{ false }
{
    
}

RenderEngine::~RenderEngine()
{
}

void RenderEngine::initialize()
{

}

void RenderEngine::destroy()
{

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
    if(windowInstance != nullptr)
    {
        m_globalDevice.initialize(windowInstance);
        m_initialized = true;
    }
}

bool RenderEngine::isInitialized() const
{
    return m_initialized;
}
