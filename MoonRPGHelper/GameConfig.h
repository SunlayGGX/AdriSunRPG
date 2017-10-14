#pragma once

/*
Put here all configurations, constants, ... you think the engine and each of its composant needs to know
*/

namespace MoonRPG
{
    enum class WindowSizeConfig : BOOL
    {
        FULL_SCREEN = FALSE,
        WINDOWED = TRUE
    };


    /*Constants*/
    enum
    {
        WINDOW_SIZE_CONFIGURATION = WindowSizeConfig::WINDOWED,

        WINDOWED_WIDTH_RESOLUTION = 1280,
        WINDOWED_HEIGTH_RESOLUTION = 720,

        ENGINE_FPS = 60,

        WINDOWS_THREAD_UPDATE_RATE_IN_MILLISECONDS = 250,
        LOGGER_THREAD_UPDATE_RATE_IN_MILLISECONDS = 250
    };
}
