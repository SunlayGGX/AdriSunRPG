/* -----------------------------------------------------------------------------
 * LoggerManager Definitions
 * ---------------------------------------------------------------------------*/

#include "MoonRPGHelperPch.h"
#include "LoggerManager.h"

using namespace MoonRPG;


// -----------------------------------------------------------------------------
// Init
// -----------------------------------------------------------------------------

LoggerManager::LoggerManager() {}
LoggerManager::~LoggerManager() {}

void LoggerManager::initialize()
{
    this->isRunning         = false;
    this->currentLogLevel   = static_cast<int8_t>(LogLevel::Debug);
    this->queueLogsFront    = &queueLogs1;
    this->queueLogsBack     = &queueLogs2;
    this->isLogingInFile    = false;

    // TODO Tmp (Use singleton later)
    LogChannelVS vs;
    LogChannelCout co;
    this->mapChannels.clear();
    //this->mapChannels.insert(LogChannel::Vs, &vs);
    //this->mapChannels.insert(LogChannel::Cout, &co);
}

void LoggerManager::destroy()
{
    this->isRunning = false;
    std::lock_guard<std::mutex> lock1(queuesFrontAccessMutex);
    std::lock_guard<std::mutex> lock2(queuesBackAccessMutex);
    this->mapChannels.clear();
    this->queueLogs1.clear();
    this->queueLogs2.clear();
}


// -----------------------------------------------------------------------------
// User methods
// -----------------------------------------------------------------------------

void LoggerManager::queueLog(LogLevel level, LogChannel::Output output, char const* message)
{
    if (this->currentLogLevel <= static_cast<int8_t>(level))
    {
        this->internalQueueLog(level, output, message);
    }
}

bool LoggerManager::run()
{
    if (!this->isRunning)
    {
        this->isRunning = true;

        std::thread
        {
            [this]() {
                while (this->isRunning)
                {
                    this->processFrontQueue();
                    this->swapQueues();
                    // Just sleep for 1 sec, otherwise, would use to many CPU
                    std::this_thread::sleep_for(std::chrono::milliseconds{1000});
                }
            }
        }.detach();

        return true;
    }
    return false;
}


// -----------------------------------------------------------------------------
// Internal methods
// -----------------------------------------------------------------------------

void LoggerManager::internalQueueLog(LogLevel level, LogChannel::Output output, std::string message)
{
    // Message passed by copie, otherwise, local scope of variable would destroye it.
    std::lock_guard<std::mutex> lock(queuesBackAccessMutex);
    this->queueLogsBack->emplace_back(level, output, std::move(message), __FILE__, __LINE__);
}

void LoggerManager::processFrontQueue()
{
    std::lock_guard<std::mutex> lock(queuesFrontAccessMutex);

    for (LogMessage& msg : *this->queueLogsFront) {
        LogChannel& channel             = this->mapChannels.at(msg.getLogChannel());
        std::string formattedMessage    = msg.getFormattedMessage();

        channel.writeInChannel(formattedMessage);
        if (this->isLogingInFile)
        {
            channel.writeInFile(formattedMessage);
        }
    }
    this->queueLogsFront->clear();
}

void LoggerManager::swapQueues()
{
    std::lock_guard<std::mutex> lock1(queuesFrontAccessMutex);
    std::lock_guard<std::mutex> lock2(queuesBackAccessMutex);
    std::swap(this->queueLogsFront, this->queueLogsBack);
}