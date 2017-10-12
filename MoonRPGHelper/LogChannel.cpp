/* -----------------------------------------------------------------------------
 * LogChannel Definitions
 * ----------------------------------------------------------------------------*/

#include "MoonRPGHelperPch.h"
#include "LoggerManager.h"

#include <string>
#include <iostream>
#include <Windows.h>

using namespace MoonRPG;


void LogChannel::writeInFile(std::string const& message)
{
    if (this->fileOutputStream.is_open())
    {
        this->fileOutputStream << message << std::endl;
    }
}

void LogChannel::setFilePath(std::string& filePath)
{
    if (this->fileOutputStream.is_open())
    {
        this->fileOutputStream.close();
    }

    this->logFilePath = filePath;
    this->fileOutputStream.open(this->logFilePath, std::ios::out);
}


void LogChannelVS::writeInChannel(std::string const& message) const
{
    OutputDebugStringA(static_cast<LPCSTR>(message.c_str()));
}

void LogChannelCout::writeInChannel(std::string const& message) const
{
    std::cout << message << std::endl;
}