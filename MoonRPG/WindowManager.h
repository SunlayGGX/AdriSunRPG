#pragma once

#include "MoonSingleton.h"


namespace MoonRPG
{
    class WindowManager : private MoonSingleton<WindowManager>
    {
    private:
        GENERATE_CODE_FROM_MoonSingleton(WindowManager);


    public:
        enum
        {
            MAX_TITLE_COUNT = 100,
        };


    private:
        HWND m_windowVisuHandle;
        TCHAR m_windowClass[MoonRPG::WindowManager::MAX_TITLE_COUNT];
        HACCEL m_accelerationTable;


    public:
        virtual void initialize() override;
        virtual void destroy() override;

        void update();

        HWND getWindowHandle() const;
        const TCHAR* getWindowTitleName() const;
        HACCEL getWindowAccelerationTable() const;
    };
}

