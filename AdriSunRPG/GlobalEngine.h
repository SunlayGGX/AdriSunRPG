#pragma once

#include "AdriSunSingleton.h"


namespace AdriSunRPG
{
    class GlobalEngine : private AdriSunSingleton<GlobalEngine>
    {
    private:
        GENERATE_CODE_FROM_AdriSunSingleton(GlobalEngine);


    public:
        std::atomic<bool> m_run;
        virtual void initialize() override;
        virtual void destroy() override;
        void quit();
    };
}

