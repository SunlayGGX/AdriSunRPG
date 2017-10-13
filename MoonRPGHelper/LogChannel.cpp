/* -----------------------------------------------------------------------------
 * LogChannel Definitions
 * ----------------------------------------------------------------------------*/

#include "MoonRPGHelperPch.h"
#include "LoggerManager.h"

#include <string>
#include <iostream>
#include <Windows.h>

using namespace MoonRPG;


LogChannel::LogChannel(std::string const& filePath)
{
    this->linkWithFile(filePath);
}

LogChannel::~LogChannel()
{
    this->unlinkFile();
}


void LogChannel::writeInFile(std::string const& message)
{
    if (this->fileOutputStream.is_open())
    {
        this->fileOutputStream << message << std::endl;
    }
}

void LogChannel::linkWithFile(std::string const& filePath)
{
    if (this->fileOutputStream.is_open())
    {
        this->fileOutputStream.close();
    }
    this->pathLogFile = filePath;
    this->fileOutputStream.open(filePath, std::ios::out | std::ios::app);
}

void LogChannel::unlinkFile()
{
    if (this->fileOutputStream.is_open())
    {
        this->fileOutputStream.close();
        this->pathLogFile.clear();
    }
}

void LogChannel::flushLogFile()
{
    if (this->fileOutputStream.is_open())
    {
        this->fileOutputStream.close();
        this->fileOutputStream.open(this->pathLogFile, std::ios::out | std::ios::trunc);
        this->fileOutputStream.close();
        this->fileOutputStream.open(this->pathLogFile, std::ios::out | std::ios::app);
        //this->fileOutputStream << "";
    }
}

/* -----------------------------------------------------------------------------
 * Special LogChannels Definitions (Children)
 * ----------------------------------------------------------------------------*/

void LogChannelVS::writeInChannel(std::string const& message) const
{
    OutputDebugStringA(static_cast<LPCSTR>(message.c_str()));
    OutputDebugStringA(static_cast<LPCSTR>("\n")); // There is probably a cleaner way
}

void LogChannelCout::writeInChannel(std::string const& message) const
{
    std::cout << message << std::endl;
}