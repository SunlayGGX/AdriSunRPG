#pragma once

#include "MoonSingleton.h"

namespace MoonRPG
{
    class InputEngine : public MoonRPG::MoonSingleton<InputEngine>
    {
    private:
        GENERATE_CODE_FROM_MoonSingleton(InputEngine);


    private:
        std::unique_ptr<class GamepadController> m_gamepadController;
        std::unique_ptr<class KeyboardController> m_keyboardController;


    public:
        virtual void initialize() override;
        virtual void destroy() override;

        void update();

        void createKeyboard(HWND windowsInstance);
    };
}
