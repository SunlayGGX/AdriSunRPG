/* -----------------------------------------------------------------------------
 * LogMessage Definitions
 * ----------------------------------------------------------------------------*/

#include "MoonRPGHelperPch.h"
#include "LoggerManager.h"

#include <sstream>
#include <string>

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
    std::chrono::time_point<std::chrono::system_clock> now;
    now                 = std::chrono::system_clock::now();
    time_t t            = std::chrono::system_clock::to_time_t(now);
    std::string dateStr = ctime(&t);
    dateStr              .resize(dateStr.size() - 1);


    std::ostringstream os;


    // Ugly switch (We could create a class for each LogLevel otherwise)
    switch (this->logLevel) {

        case LogLevel::Error:
            os << "[" << dateStr << "]" << " [ERROR]: " << this->message;
            break;
        case LogLevel::Warning:
            os << "[" << dateStr << "]" << " [WARNING]: " << this->message;
            break;
        case LogLevel::Config:
            os << "[" << dateStr << "]" << " [CONFIG]: " << this->message;
            break;
        case LogLevel::Info:
            os << "[" << dateStr << "]" << " [INFO]: " << this->message;
            break;
        case LogLevel::Trace:
            os << "[" << dateStr << "]" << " [TRACE]: " << this->message;
            break;
        case LogLevel::Debug:
            os << "[" << dateStr << "] [" << this->filePosition << ": " << this->linePosition << "] [DEBUG]: " << this->message;
            break;
        default:
            os << "[" << dateStr << "]" << " [Unknown LogLevel]: " << this->message;
            break;
    }

    return os.str();
}