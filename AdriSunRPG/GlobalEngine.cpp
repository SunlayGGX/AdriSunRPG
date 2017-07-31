#include "GlobalEngine.h"


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

void GlobalEngine::quit()
{
    m_run = false;
}
