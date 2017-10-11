/* -----------------------------------------------------------------------------
 * LogMessage Definitions
 * ----------------------------------------------------------------------------*/

#include "MoonRPGHelperPch.h"
#include "LoggerManager.h"

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

    //TODO (With std::chrono)

    /*
    * Dev note: all LogMessages share the same statc formatedMessage.
    * This is for space use, but require to use the result before
    * calling this function for another message.
    */

    /*
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
    */
    return this->message;
}