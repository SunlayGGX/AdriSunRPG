#pragma once

#include <fstream>
#include <unordered_map>
#include <mutex>
#include <atomic>
#include <ctime>

#include "MoonSingleton.h"


// -----------------------------------------------------------------------------
// End user macros
// -----------------------------------------------------------------------------

#define LOG_VS_ERROR(msg)       MoonRPG::LoggerManager::instance().queueLog(MoonRPG::LogLevel::Error,  MoonRPG::LogChannel::Vs, msg)
#define LOG_VS_WARNING(msg)     MoonRPG::LoggerManager::instance().queueLog(MoonRPG::LogLevel::Warning,MoonRPG::LogChannel::Vs, msg)
#define LOG_VS_CONFIG(msg)      MoonRPG::LoggerManager::instance().queueLog(MoonRPG::LogLevel::Config, MoonRPG::LogChannel::Vs, msg)
#define LOG_VS_INFO(msg)        MoonRPG::LoggerManager::instance().queueLog(MoonRPG::LogLevel::Info,   MoonRPG::LogChannel::Vs, msg)
#define LOG_VS_TRACE(msg)       MoonRPG::LoggerManager::instance().queueLog(MoonRPG::LogLevel::Trace,  MoonRPG::LogChannel::Vs, msg)
#define LOG_VS_DEBUG(msg)       MoonRPG::LoggerManager::instance().queueLog(MoonRPG::LogLevel::Debug,  MoonRPG::LogChannel::Vs, msg)

#define LOG_COUT_ERROR(msg)     MoonRPG::LoggerManager::instance().queueLog(MoonRPG::LogLevel::Error,  MoonRPG::LogChannel::Cout, msg)
#define LOG_COUT_WARNING(msg)   MoonRPG::LoggerManager::instance().queueLog(MoonRPG::LogLevel::Warning,MoonRPG::LogChannel::Cout, msg)
#define LOG_COUT_CONFIG(msg)    MoonRPG::LoggerManager::instance().queueLog(MoonRPG::LogLevel::Config, MoonRPG::LogChannel::Cout, msg)
#define LOG_COUT_INFO(msg)      MoonRPG::LoggerManager::instance().queueLog(MoonRPG::LogLevel::Info,   MoonRPG::LogChannel::Cout, msg)
#define LOG_COUT_TRACE(msg)     MoonRPG::LoggerManager::instance().queueLog(MoonRPG::LogLevel::Trace,  MoonRPG::LogChannel::Cout, msg)
#define LOG_COUT_DEBUG(msg)     MoonRPG::LoggerManager::instance().queueLog(MoonRPG::LogLevel::Debug,  MoonRPG::LogChannel::Cout, msg)

#define LOG_ERROR(msg)          LOG_VS_ERROR(msg)
#define LOG_WARNING(msg)        LOG_VS_WARNING(msg)
#define LOG_CONFIG(msg)         LOG_VS_CONFIG(msg)
#define LOG_INFO(msg)           LOG_VS_INFO(msg)
#define LOG_TRACE(msg)          LOG_VS_TRACE(msg)
#define LOG_DEBUG(msg)          LOG_VS_DEBUG(msg)


namespace MoonRPG
{

    /**
     * Defines the level of logs.
     * Lowest value is for more critical logs. (Error < DEBUG).
     * int8_t is only for little place optim + use with atomic_int8_t.
     */
    enum class LogLevel : int8_t
    {
        Error,      // Lowest Log level: Critical.
        Warning,    // Log that is not critical but may represent a threat
        Config,     // Configuration log
        Info,       // Informative log about engine execution
        Trace,      // Informative log that are less important
        Debug       // For debug
    };


    // -------------------------------------------------------------------------
    // LOG CHANNELS
    // -------------------------------------------------------------------------

    /**
     * Abstract class that defines a log channel.
     * A log channel is an output where logs can be displayed.
     *
     * \remark
     * All available log channels are listed by LogChannel::Output enum.
     * To reference the channel COCO, use LogChannel::COCO .
     *
     * \remark
     * Channel may be associated with a file to be used if logs are
     * written in files.
     *
     * \note
     * HOW TO ADD A NEW LOG CHANNEL IN YOUR LOGGERS LIST:
     * To implement a new LogChannel:
     *  inherit this class, 
     *  add new constant in LogChannel::Output,
     *  register this new channel in the LoggerManager.
     */
    class LogChannel
    {
        public:
            /** List of available channels. */
            enum Output
            {
                Vs,         // Visual Studio
                Cout        // Simple std::cout
            };


        protected:

            /**
             * Name of the file associated with this channel.
             * (Full path + name + ext)
             */
            std::string logFilePath;

            /**
             * Stream used to write in file.
             */
            std::ofstream fileOutputStream;

        public:

            /**
             * Write a message in this channel and add a line return.
             * Message is printed as it is.
             *
             * \param message The message to print in the channel.
             */
            virtual void writeInChannel(std::string const& message) const = 0;

            /**
             * Write a message in the associated file and add a line return.
             * Message is printed as it is (Not format added etc).
             * Do nothing no file is set (stream not open).
             *
             * \param message The message to print in the channel.
             */
            void writeInFile(std::string const& message);


        public:

            /**
             * Change the value of the logFilePath and open the new stream.
             */
            void setFilePath(std::string& filePath);
    };

    /**
     * LogChannel implementation for Visual Studio output.
     */
    class LogChannelVS : public LogChannel
    {
        public:
            void writeInChannel(std::string const& message) const override;
    };

    /**
     * LogChannel implementation for std::cout output.
     */
    class LogChannelCout : public LogChannel
    {
        public:
            void writeInChannel(std::string const& message) const override;
    };


    // -------------------------------------------------------------------------
    // LOG MESSAGE
    // -------------------------------------------------------------------------

    /**
     * Internal representation of a log message.
     */
    class LogMessage {

        private:
            LogLevel				logLevel;
            LogChannel::Output		channel;
            std::string		        message;

            std::string             filePosition;
            const int               linePosition;
            const time_t            creationTime;

        public:
            LogMessage(const LogLevel logLevel,
                       const LogChannel::Output channel,
                       std::string&& message,
                       std::string&& file,
                       const int line);


        public:

            /**
             * Returns the Formatted version of the message.
             * (Message is truncated if length highter than max size).
             * 
             * \warning
             * Not thread safe, only one formatedMessage can be set (Static variable).
             *
             * \return Pointer to the static variable that has the formated message.
             */
            const std::string getFormattedMessage() const;

            /** Returns the logChannel associated with this LogMessage */
            LogChannel::Output getLogChannel() const
            {
                return this->channel;
            }
    };


    // -------------------------------------------------------------------------
    // LOG MANAGER
    // -------------------------------------------------------------------------

    /**
     * The main logger manager (Singleton).
     * All functions are thread safe.
     *
     * \remark
     * By default, logs are only displayed in channels.
     * logInFileMode may be set to also write all logs in files.
     * 
     * \remark
     * Log level may be set to choose which logs are bypassed.
     * Only logs with lower or equal enum value are displayed.
     * (See LogLevel Enum).
     *
     * \author  Constantin Masson
     * \date    Oct, 2017
     */
    class LoggerManager : private MoonSingleton<LoggerManager>
    {

        private:
            // -----------------------------------------------------------------
            // Attributs
            // -----------------------------------------------------------------

            /** True if this Logger is Running */
            std::atomic_bool isRunning;

            /** The current used log level (From LogLevel enum). */
            std::atomic_int8_t currentLogLevel; // LogLevel type

            /** If true, all logs are also written in files. */
            std::atomic_bool isLogingInFile;

            /** Path the the folder with logs. */
            std::string logFilePath;

            /** Access mutex for logFilePath */
            std::mutex logFilePathAccessMutex;

            /** Map of all registered and available output channels. */
            std::unordered_map<
                LogChannel::Output,
                std::unique_ptr<LogChannel>
            > lookupChannels;

            /** Vector of logs */
            std::vector<LogMessage>		queueLogs1;
            std::vector<LogMessage>		queueLogs2;

            /** Point to the vector currently processed by the Logger. */
            std::vector<LogMessage>*	queueLogsFront;

            /** Point to the vector where logs are queued. */
            std::vector<LogMessage>*	queueLogsBack;

            /** Mutex for Front log queues access */
            std::mutex queuesFrontAccessMutex;

            /** Mutex for Back log queues access */
            std::mutex queuesBackAccessMutex;


        private:
            // -----------------------------------------------------------------
            // Singleton implementation / override
            // -----------------------------------------------------------------
            GENERATE_CODE_FROM_MoonSingleton(LoggerManager);

        public:
            void initialize() override;
            void destroy() override;


        public:
            // -----------------------------------------------------------------
            // Core End User Methods
            // -----------------------------------------------------------------

            /**
             * Queue a Log if level is accepted by current Log settings.
             * Log is queued to be written in the respective LogChannel.
             * Current loggerManager level is used to bypass logs with superior
             * level (Less critical logs) than current level.
             *
             * \remark
             * This function is thread safe and may be called concurrently.
             *
             * \param level     Log Level for this message.
             * \param output    Channel to use with this message.
             * \param message   The row message to display.
             */
            void queueLog(LogLevel level, LogChannel::Output output,
                          char const* message);

            /**
             * Run the LoggerEngine in a new thread.
             * LoggerEngine can be started only if it's not already running.
             *
             * \warning
             * Not thread safe, should be called in one thread.
             * (In a really unlucky scenario, they might be a TOCTTOU:
             * run may be started in both threads).
             *
             * \return True is successfully started, otherwise, return false.
             */
            bool run();


        private:
            // -----------------------------------------------------------------
            // Internal methods
            // -----------------------------------------------------------------

            /**
             * Queue a log, regardless any settings.
             */
            void internalQueueLog(LogLevel level, LogChannel::Output output,
                                  std::string message);

            /**
             * Process each elements from the front queue and clear it.
             */
            void processFrontQueue();

            /**
             * Swap front and back queue buffers.
             */
            void swapQueues();

        public:
            // -----------------------------------------------------------------
            // Getters - Setters
            // -----------------------------------------------------------------

            /** Changes the current log level. */
            void setLogLevel(const LogLevel level)
            {
                this->currentLogLevel = static_cast<int8_t>(level);
            }

            /** Returns the current log level. */
            LogLevel getLogLevel() const
            {
                return static_cast<LogLevel>(this->currentLogLevel.load());
            }

            /** Logs are no longer put in files. (Path to log file is not changed). */
            void disableLogInFile()
            {
                this->isLogingInFile = false;
            }

            /** Logs are put in files. (Use log file path currently set). */
            void enableLogInFile()
            {
                this->isLogingInFile = true;
            }

            /** Set the path to use for logs. */
            void setLogFilePath(std::string const& newPathToLog)
            {
                std::lock_guard<std::mutex> lock(logFilePathAccessMutex);
                this->logFilePath = newPathToLog;
            }

    }; // End LoggerManager


} // End MoonRPG Namespace