/* -----------------------------------------------------------------------------
 * LoggerManager Definitions
 * ---------------------------------------------------------------------------*/

#include "MoonRPGHelperPch.h"
#include "LoggerManager.h"

#include "GameConfig.h"

#include <ctime>
#include <experimental/filesystem>

using namespace MoonRPG;


// -----------------------------------------------------------------------------
// Init
// -----------------------------------------------------------------------------

LoggerManager::LoggerManager() {}
LoggerManager::~LoggerManager() {}

void LoggerManager::initialize()
{
    if (this->isRunning)
    {
        return;
    }

    this->isRunning         = true;
    this->m_queueLogsFront  = &queueLogs1;
    this->m_queueLogsBack   = &queueLogs2;
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

        if (INTERNAL_LOG_SETTINGS_ERASE_LOG_FILE_AT_START) 
        {
            if (std::experimental::filesystem::exists(logRootPath))
            {
                //TODO
                //std::experimental::filesystem::remove(logRootPath);
            }

            std::experimental::filesystem::create_directory(logRootPath);
        }
    }

    this->internalStartLoggerThread();
}

void LoggerManager::internalStartLoggerThread()
{
    std::thread
    {
        [this]() {
        while (this->isRunning)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds{MoonRPG::LOGGER_THREAD_UPDATE_RATE_IN_MILLISECONDS});
            this->processBackQueue();
            this->swapQueues();
        }
    }
    }.detach();
}


void LoggerManager::destroy()
{
    this->isRunning = false;
    this->processBackQueue();
    this->swapQueues();
    this->processBackQueue();
    this->isLogingInFile = false;
}

void LoggerManager::saveFileToSafeDirectory(const std::string &safeDirectory) const
{
    // TODO
    //std::experimental::filesystem::directory_iterator iter{INTERNAL_LOG_SETTINGS_DEFAULT_LOGPATH};
    //std::experimental::filesystem::copy(INTERNAL_LOG_SETTINGS_DEFAULT_LOGPATH, safeDirectory);
}

// -----------------------------------------------------------------------------
// User methods
// -----------------------------------------------------------------------------

void LoggerManager::queueLog(LogLevel level, LogChannel::Output output,
                             char const* message,
                             char const* file,
                             int line)
{
    if (this->isRunning && this->currentLogLevel.load() >= static_cast<int8_t>(level))
    {
        this->internalQueueLog(level, output, message, file, line);
    }
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
    std::lock_guard<std::mutex> lock(m_queuesFrontAccessMutex);
    this->m_queueLogsFront->emplace_back(level, output, std::move(message), std::move(file), line);
}

void LoggerManager::processBackQueue()
{
    for (LogMessage& msg : *this->m_queueLogsBack) 
    {
        std::string formattedMessage = msg.getFormattedMessage();

        auto& coco = lookupChannels[msg.getLogChannel()];
        coco->writeInChannel(formattedMessage);
        if (this->isLogingInFile)
        {
            coco->writeInFile(formattedMessage);
        }
    }
    this->m_queueLogsBack->clear();
}

void LoggerManager::swapQueues()
{
    std::lock_guard<std::mutex> lock1(m_queuesFrontAccessMutex);
    std::swap(this->m_queueLogsFront, this->m_queueLogsBack);
}