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
    this->isRunning         = false; // Don't set true, run would faild.
    this->queueLogsFront    = &queueLogs1;
    this->queueLogsBack     = &queueLogs2;
    this->isLogingInFile    = INTERNAL_LOG_SETTINGS_DEFAULT_IS_LOGIN_IN_FILE;
    this->currentLogLevel   = static_cast<int8_t>(INTERNAL_LOG_SETTINGS_DEFAULT_LOG_LEVEL);

    // Register all available log channels. Ifnew channels created, add here.

    this->lookupChannels.insert(std::make_pair(LogChannel::Vs, std::unique_ptr<LogChannelVS>(new LogChannelVS())));
    this->lookupChannels.insert(std::make_pair(LogChannel::Cout, std::unique_ptr<LogChannelCout>(new LogChannelCout())));

    if (this->isLogingInFile)
    {
        std::string logRootPath = INTERNAL_LOG_SETTINGS_DEFAULT_LOGPATH;
        std::string vsLogPath = logRootPath + INTERNAL_LOG_SETTINGS_FILE_SEPARATOR + "vs.log";
        std::string coutLogPath = logRootPath + INTERNAL_LOG_SETTINGS_FILE_SEPARATOR + "cout.log";

        lookupChannels[LogChannel::Vs]->linkWithFile(vsLogPath);
        lookupChannels[LogChannel::Cout]->linkWithFile(coutLogPath);

        if (INTERNAL_LOG_SETTINGS_FLUSH_LOG_FILE_AT_START) {
            lookupChannels[LogChannel::Vs]->flushLogFile();
            lookupChannels[LogChannel::Cout]->flushLogFile();
        }
    }
}


void LoggerManager::destroy()
{
    this->isRunning = false;
    this->isLogingInFile = false;
    std::lock_guard<std::mutex> lock1(queuesFrontAccessMutex);
    std::lock_guard<std::mutex> lock2(queuesBackAccessMutex);
    this->lookupChannels.clear();
    this->queueLogs1.clear();
    this->queueLogs2.clear();
}


// -----------------------------------------------------------------------------
// User methods
// -----------------------------------------------------------------------------

void LoggerManager::queueLog(LogLevel level, LogChannel::Output output,
                             char const* message,
                             char const* file,
                             int line)
{
    if (this->currentLogLevel.load() >= static_cast<int8_t>(level))
    {
        this->internalQueueLog(level, output, message, file, line);
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
                    if (this->queueLogsFront->empty()) {
                        // Just sleep for 1 sec, otherwise, would use to many CPU
                        std::this_thread::sleep_for(std::chrono::milliseconds{1000});
                    }
                    else
                    {
                        std::this_thread::sleep_for(std::chrono::milliseconds{100});
                    }
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

void LoggerManager::internalQueueLog(LogLevel level, LogChannel::Output output,
                                     std::string message,
                                     std::string file, 
                                     const int line)
{
    // Message passed by copie, otherwise, local scope of variable would destroye it.
    std::lock_guard<std::mutex> lock(queuesBackAccessMutex);
    this->queueLogsBack->emplace_back(level, output, std::move(message), std::move(file), line);
}

void LoggerManager::processFrontQueue()
{
    std::lock_guard<std::mutex> lock(queuesFrontAccessMutex);

    for (LogMessage& msg : *this->queueLogsFront) {

        std::string formattedMessage = msg.getFormattedMessage();

        lookupChannels[msg.getLogChannel()]->writeInChannel(formattedMessage);
        if (this->isLogingInFile)
        {
            lookupChannels[msg.getLogChannel()]->writeInFile(formattedMessage);
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