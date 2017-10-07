#pragma once

#include "MoonSingleton.h"


namespace MoonRPG
{
    class GlobalEngine : private MoonSingleton<GlobalEngine>
    {
    private:
        GENERATE_CODE_FROM_MoonSingleton(GlobalEngine);


    public:
        std::atomic<bool> m_run;


    public:
        virtual void initialize() override;
        virtual void destroy() override;

        void run();

        void quit();

        bool isFullyInitialized() const;
    };
}

