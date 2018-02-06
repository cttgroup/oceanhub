#ifndef LOGGER_H
#define LOGGER_H

#include <ctime>
#include <iostream>


class LoggerStream {
public:
    bool init(const std::string &filepath);
    static LoggerStream * getInstance();
    std::ostream * streamPointer();
private:
    static LoggerStream *pInstance;
    LoggerStream();
    std::ostream * stream;

};

#ifdef _MSC_VER
#define FUINFO __FUNCSIG__
#endif
#ifdef __GNUC__
#define FUINFO __PRETTY_FUNCTION__
#endif

#define LOG(X) { \
    std::time_t result = std::time(nullptr); \
    if (LoggerStream::getInstance() && LoggerStream::getInstance()->streamPointer()) \
    { \
        std::ostream &ostr = *LoggerStream::getInstance()->streamPointer(); \
        ostr \
        << std::asctime(std::localtime(&result)) \
        << X << std::endl; \
    } \
    std::cout \
    << std::asctime(std::localtime(&result)) \
    << X << std::endl; \
    }

#endif // LOGGER_H
