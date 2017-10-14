/* -----------------------------------------------------------------------------
 * LogMessage Definitions
 * ---------------------------------------------------------------------------*/

#include "MoonRPGHelperPch.h"
#include "LoggerManager.h"

#include <ctime>

using namespace MoonRPG;


LogMessage::LogMessage(const LogLevel logLevel,
                       const LogChannel::Output channel,
                       std::string&& message,
                       std::string&& file,
                       const int line)
    : logLevel(logLevel),
      channel(channel),
      message(std::move(message)),
      filePosition(std::move(file)),
      linePosition(line),
      creationTime(std::time(0))
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

    std::string temp = "[" + dateStr +"] ";

    if (this->logLevel == LogLevel::Debug)
    {
        temp += "[" + this->filePosition + ": " + std::to_string(this->linePosition) + "] ";
    }

    return temp + formatLU[static_cast<size_t>(this->logLevel)] + this->message;
}