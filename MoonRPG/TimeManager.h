#pragma once

#include "MoonSingleton.h"

#include "Scheduler.h"

namespace MoonRPG
{
    class TimeManager : public MoonRPG::MoonSingleton<TimeManager>
    {
    private:
        GENERATE_CODE_FROM_MoonSingleton(TimeManager);


    private:
        std::chrono::time_point<std::chrono::steady_clock> m_engineStart;
        std::chrono::time_point<std::chrono::steady_clock> m_gameStart;
        std::chrono::time_point<std::chrono::steady_clock> m_frameStart;
        std::chrono::time_point<std::chrono::steady_clock> m_approximativeFrameEnd;

        std::chrono::microseconds m_frameDuration;
        
        std::atomic<float> m_lastFrameDurationInSeconds;

        mutable std::mutex m_syncMutex;
        std::condition_variable m_synchronizer;

        std::atomic<bool> m_canHookOnTimeManager;
        std::atomic<bool> m_run;

        std::vector<LoopScheduler> m_loopSchedulerArray;
        mutable std::mutex m_mutex;


    public:
        virtual void initialize() override;
        virtual void destroy() override;

        float getLastFrameDurationInSeconds() const;

        std::chrono::nanoseconds getRemainingFrameTime() const;
        std::chrono::nanoseconds getElapsedTimeSinceEngineStarting() const;
        std::chrono::nanoseconds getElapsedTimeSinceGameStarting() const;

        void startGame();

        void waitEndOfFrame();


    public:
        template<class TimeLoop, class Algorithm, class ... Args>
        void addLoopScheduler(const bool tightLoop, TimeLoop time, Algorithm&& func, Args&& ... args)
        {
            if(m_canHookOnTimeManager)
            {
                std::lock_guard<std::mutex> autoLocker{ m_mutex };
                m_loopSchedulerArray.emplace_back(tightLoop, time, std::forward<Algorithm>(func), std::forward<Args>(args)...);
            }
        }


    private:
        void internalUpdateTimes();
    };
}
