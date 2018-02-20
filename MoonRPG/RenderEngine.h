#pragma once

#include "MoonSingleton.h"

#include "DirectXGlobalDevice.h"


namespace MoonRPG
{
    class RenderEngine : private MoonSingleton<RenderEngine>
    {
    private:
        GENERATE_CODE_FROM_MoonSingleton(RenderEngine);


    private:
        bool m_initialized;
        DirectXGlobalDevice m_globalDevice;


    public:
        virtual void initialize() override;
        virtual void destroy() override;

        void update();

        void setWindowsHandleInstance(HWND windowInstance);

        bool isInitialized() const;


    public:
        FORCEINLINE const DirectXGlobalDevice& getGlobalDevice() const
        {
            return m_globalDevice;
        }
    };
}


