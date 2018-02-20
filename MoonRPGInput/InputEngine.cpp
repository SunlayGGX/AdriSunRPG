#include "InputEngine.h"

#include "GamepadController.h"
#include "KeyboardController.h"

#include "ElephantLogger.h"

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
        LOG_ERROR("No Gamepad connected");
    }

    m_keyboardController->update();
}

void InputEngine::createKeyboard(HWND windowsInstance)
{
    m_keyboardController->createKeyboard(windowsInstance);
}

KeyboardElement InputEngine::getCurrentKeyboardState() const
{
    return m_keyboardController->getCurrentKeyboardState();
}

KeyboardElement InputEngine::getKeyboardChangedState() const
{
    return m_keyboardController->getKeyboardChangedState();
}

KeyboardElement InputEngine::getKeyboardUpState() const
{
    return m_keyboardController->getKeyboardUpState();
}

KeyboardElement InputEngine::getKeyboardDownState() const
{
    return m_keyboardController->getKeyboardDownState();
}

Gamepad InputEngine::getFirstGamepadState() const
{
    return m_gamepadController->getFirstGamepad();
}
