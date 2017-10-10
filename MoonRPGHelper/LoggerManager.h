#pragma once

#include <iostream>
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
     * Lowest value is for more critical logs. (Error < DEBUG)
     */
    enum class LogLevel
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
     * To implement a new LogChannel:
     *  inherit this class, 
     *  add new constant in LogChannel::Output,
     *  register this new channel in the LoggerManager.
     */
    class LogChannel
    {
        public:

            /** List of available channels */
            enum Output
            {
                Vs,         // Visual Studio
                Cout        // Simple std::cout
            };


        protected:

            /** Name of the file associated with this channel. (Full path + name + ext) */
            char *filePath = nullptr;


        protected:

            LogChannel() = default;
            explicit LogChannel(char *filePath) : filePath(filePath) {}
            virtual ~LogChannel() = default;

        public:

            /**
             * Write a message in this channel and add a line return.
             * Message is printed as it is.
             *
             * \param message The message to print in the channel.
             */
            virtual void writeInChannel(char const* message) const = 0;

            /**
             * Write a message in the associated file and add a line return.
             * Message is printed as it is.
             * If no file is linked with this channel, do nothing.
             *
             * \param message The message to print in the channel.
             */
            void writeInFile(char const* message) const
            {
                if (this->filePath != nullptr)
                {
                    std::ofstream os;
                    os.open(this->filePath);
                    os << message << std::endl;
                    os.close();
                }
            }


        public:
            void setFilePath(char *filePath)
            {
                this->filePath = filePath;
            }
    };


    /**
     * LogChannel implementation for Visual Studio output.
     */
    class LogChannelVS : public LogChannel {
        public:
            void writeInChannel(char const* message) const
            {
                std::cout << "[TODO VS Not implemented] " << message << std::endl;
                //OutputDebugString((LPCWSTR)message);
            }
    };


    /**
     * LogChannel implementation for std::cout output.
     */
    class LogChannelCout : public LogChannel
    {
        public:
            void writeInChannel(char const* message) const
            {
                std::cout << message << std::endl;
            }
    };


    // -------------------------------------------------------------------------
    // LOG MESSAGE
    // -------------------------------------------------------------------------

    /**
     * Internal representation of a log message.
     */
    class LogMessage {

        private:

            /** Max size of a log message (Debug format included) */
            static const int        MAX_MESSAGE_SIZE = 255;
            static char             formattedMessage[MAX_MESSAGE_SIZE + 1];

            LogLevel				logLevel;
            LogChannel::Output		channel;
            char const*		        message;
            const char*             filePosition;
            const int               linePosition;
            const time_t            creationTime;

        public:
            LogMessage(const LogLevel logLevel, const LogChannel::Output channel, char const* message)
                : logLevel(logLevel),
                  channel(channel),
                  message(message),
                  creationTime(std::time(0)),
                  filePosition(__FILE__),
                  linePosition(__LINE__)
            {}


        public:
            LogChannel::Output getLogChannel() const
            {
                return this->channel;
            }

            /**
             * Returns the Formatted version of the message.
             * (Message is truncated if length highter than max size).
             * 
             * \warning
             * Not thread safe, only one formatedMessage can be set (Static variable).
             *
             * \return Pointer to the static variable that has the formated message.
             */
            char const* getFormattedMessage() const
            {
                /*
                 * Dev note: all LogMessages share the same statc formatedMessage.
                 * This is for space use, but require to use the result before
                 * calling this function for another message.
                 */

                // Ugly switch (We could create a class for each LogLevel otherwise)
                switch (this->logLevel) {

                    case LogLevel::Error:
                        snprintf(formattedMessage, MAX_MESSAGE_SIZE,
                                 "[%s] [ERROR]: %s",
                                 std::ctime(&creationTime), message);
                        break;
                    case LogLevel::Warning:
                        snprintf(formattedMessage, MAX_MESSAGE_SIZE,
                                 "[%s] [WARNING]: %s",
                                 std::ctime(&creationTime), message);
                        break;
                    case LogLevel::Config:
                        snprintf(formattedMessage, MAX_MESSAGE_SIZE,
                                 "[%s] [CONFIG]: %s",
                                 std::ctime(&creationTime), message);
                        break;
                    case LogLevel::Info:
                        snprintf(formattedMessage, MAX_MESSAGE_SIZE,
                                 "[%s] [INFO]: %s",
                                 std::ctime(&creationTime), message);
                        break;
                    case LogLevel::Trace:
                        snprintf(formattedMessage, MAX_MESSAGE_SIZE,
                                 "[%s : %d] [TRACE]: %s",
                                 filePosition, linePosition, message);
                        break;
                    case LogLevel::Debug:
                        snprintf(formattedMessage, MAX_MESSAGE_SIZE,
                                 "[%s : %d] [DEBUG]: %s",
                                 filePosition, linePosition, message);
                        break;
                    default:
                        snprintf(formattedMessage, MAX_MESSAGE_SIZE,
                                 "[%s : %d] [Unknown LogLevel]: %s",
                                 filePosition, linePosition, message);
                        break;
                }

                return formattedMessage;
            }
    };


    // -------------------------------------------------------------------------
    // LOG MANAGER
    // -------------------------------------------------------------------------

    /**
     * The main logger manager (Singleton).
     *
     * \remark
     * By default, logs are only displayed in channels.
     * logInFileMode may be set to also write all logs in files.
     * 
     * \remark
     * log level may be set to choose which logs are bypassed.
     * Only logs with lower or equal enum value are displayed.
     * (See LogLevel Enum).
     *
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

            std::atomic_bool isRunning; // Atomic in case run called simultaneously

            /** If true, all logs are also written in files. */
            bool logInFileMode;

            /** Path the the folder with logs. */
            char *logPath;

            /** The current used log level. */
            LogLevel currentLogLevel;

            /** Map of all registered and available output channels. */
            std::unordered_map<LogChannel::Output, LogChannel&> mapChannels;

            std::vector<LogMessage>		queueLogs1;
            std::vector<LogMessage>		queueLogs2;
            std::vector<LogMessage>*	queueLogsFront; // We process these logs
            std::vector<LogMessage>*	queueLogsBack; // We queue logs here

            std::mutex queueMutex;


        private:
            // -----------------------------------------------------------------
            // Singleton implementation / override
            // -----------------------------------------------------------------
            GENERATE_CODE_FROM_MoonSingleton(LoggerManager);

            void initialize()
            {
                this->isRunning			= false;
                this->currentLogLevel	= LogLevel::Debug;
                this->queueLogsFront	= &queueLogs1;
                this->queueLogsBack		= &queueLogs2;
                this->logInFileMode     = false;

                // TODO Tmp (Use singleton later)
                LogChannelVS vs;
                LogChannelCout co;
                this->mapChannels.clear();
                //this->mapChannels.insert(LogChannel::Vs, &vs);
                //this->mapChannels.insert(LogChannel::Cout, &co);
            }

            void destroy()
            {
                this->isRunning = false;
                this->mapChannels.clear();
                this->queueLogs1.clear();
                this->queueLogs2.clear();
            }


        public:
            // -----------------------------------------------------------------
            // Core End User Methods
            // -----------------------------------------------------------------

            /**
             * Queue a Log.
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
            void queueLog(LogLevel level, LogChannel::Output output, char const* message)
            {
                if (this->currentLogLevel <= level)
                {
                    std::lock_guard<std::mutex> lock(queueMutex);
                    this->queueLogsBack->emplace_back(level, output, message);
                }
            }

            /**
             * Run the LoggerEngine in a new thread.
             * LoggerEngine can be started only if it's not already running.
             *
             * \return True is successfully started, otherwise, return false.
             */
            bool run()
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
                                //TODO Maybe wait for front queue to be not empty
                            }
                        }
                    }.detach();

                    return true;
                }
                return false;
            }


        private:
            // -----------------------------------------------------------------
            // Internal methods
            // -----------------------------------------------------------------

            /**
             * Process each elements from the front queue and clear it.
             *
             * \remark
             * Not thread safe, only used internally.
             */
            void processFrontQueue()
            {
                for (LogMessage& msg : *this->queueLogsFront) {

                    LogChannel const& channel = this->mapChannels.at(msg.getLogChannel());
                    channel.writeInChannel(msg.getFormattedMessage());

                    if (this->logInFileMode)
                    {
                        channel.writeInFile(msg.getFormattedMessage());
                    }
                }
                this->queueLogsFront->clear();
            }

            /**
             * Swap front and back queue buffers.
             * Thread safe.
             */
            void swapQueues()
            {
                std::lock_guard<std::mutex> lock(queueMutex);
                std::vector<LogMessage>* ptr;
                ptr = this->queueLogsFront;
                this->queueLogsFront = this->queueLogsBack;
                this->queueLogsBack = ptr;
            }

        public:
            // -----------------------------------------------------------------
            // Getters - Setters
            // -----------------------------------------------------------------

            /** Change the current log level. */
            void setLogLevel(const LogLevel level)
            {
                this->currentLogLevel = level;
            }

            /** Returns the current log level. */
            LogLevel getLogLevel() const
            {
                return this->currentLogLevel;
            }

            /** Logs are no longer put in files */
            void disableLogInFile()
            {
                this->logInFileMode = false;
            }

            void activateLogInFile(char * path)
            {
                this->logPath = path;
                this->logInFileMode = true;
            }

    }; // LoggerManager


} // End MoonRPG Namespace