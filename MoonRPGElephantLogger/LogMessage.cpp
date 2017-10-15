/* -----------------------------------------------------------------------------
 * LogMessage Definitions
 * ---------------------------------------------------------------------------*/

#include "MoonRPGElephantLoggerPch.h"

#include "LogMessage.h"

#include <ctime>

using namespace MoonRPG;


LogMessage::LogMessage(const LogLevel logLevel,
                       const LogChannel::Output channel,
                       std::string&& message,
                       std::string&& file,
                       const int line)
    : m_logLevel(logLevel),
      m_channel(channel),
      m_message(std::move(message)),
      m_filePosition(std::move(file)),
      m_linePosition(line),
      m_creationTime(std::time(0))
{}

const std::string LogMessage::getFormattedMessage() const
{
    using Clock = std::chrono::system_clock;

    std::chrono::time_point<Clock> now = Clock::now();
    time_t t = Clock::to_time_t(now);
    std::string dateStr = ctime(&t);
    dateStr.pop_back(); // Remove line return

    static const std::string formatLU[] =
    {
        "[ERROR]: ",
        "[WARNING]: ",
        "[CONFIG]: ",
        "[INFO]: ",
        "[TRACE]: ",
        "[DEBUG]: ",
        "[Unknown LogLevel]: "
    };

    std::string logDate = "[" + dateStr +"] ";

    if (this->m_logLevel == LogLevel::Debug)
    {
        return logDate
            + formatLU[static_cast<size_t>(this->m_logLevel)] 
            + "[" + this->m_filePosition + ": " + std::to_string(this->m_linePosition) + "] "
            + this->m_message;
    }

    return logDate + formatLU[static_cast<size_t>(this->m_logLevel)] + this->m_message;
}

const LogChannel::Output LogMessage::getLogChannel() const
{
    return this->m_channel;
}

