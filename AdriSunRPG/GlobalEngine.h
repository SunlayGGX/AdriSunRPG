#pragma once

#include "AdriSunSingleton.h"


namespace AdriSunRPG
{
    class GlobalEngine : private AdriSunSingleton<GlobalEngine>
    {
    private:
        GENERATE_CODE_FROM_AdriSunSingleton(GlobalEngine);


    public:
        virtual void initialize() override;
        virtual void destroy() override;
    };
}

