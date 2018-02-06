#include <iostream>
#include <fstream>
#include <ios>

#include "logger.h"

LoggerStream* LoggerStream::pInstance = NULL;

LoggerStream::LoggerStream()
{
    stream = NULL;
}

bool LoggerStream::init(const std::string &filepath)
{
    stream = new std::ofstream(filepath, std::ios_base::out|std::ios_base::app);
    return true;
}

LoggerStream * LoggerStream::getInstance()
{
    if (!pInstance)
        pInstance = new LoggerStream();
    return pInstance;
}

std::ostream * LoggerStream::streamPointer()
{
    return stream;
}

