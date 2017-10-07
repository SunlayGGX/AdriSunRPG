#include "InputEngine.h"

#include "GamepadController.h"
#include "KeyboardController.h"

using namespace MoonRPG;

InputEngine::InputEngine() :
    m_gamepadController{ std::make_unique<MoonRPG::GamepadController>() },
    m_keyboardController{ std::make_unique<MoonRPG::KeyboardController>() }
{
}

InputEngine::~InputEngine()
{

}

void InputEngine::initialize()
{
    
}

void InputEngine::destroy()
{

}

void InputEngine::update()
{
    if(!m_gamepadController->update())
    {
        //TODO: Log
    }

    m_keyboardController->update();
}

void InputEngine::createKeyboard(HWND windowsInstance)
{
    m_keyboardController->createKeyboard(windowsInstance);
}
