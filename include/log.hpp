/**
 * @file log.hpp
 * @author Mateus Lima (mateuslima.ti@gmail.com)
 * @brief Log class headers.
 * @version 0.1
 * @date 2022-02-15
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __LOG_HPP__
#define __LOG_HPP__

#include <streambuf>
#include <syslog.h>

/**
 * @brief Log priority enumeration.
 *
 */
enum LogPriority
{
    LogEmerg = LOG_EMERG,     // system is unusable
    LogAlert = LOG_ALERT,     // action must be taken immediately
    LogCrit = LOG_CRIT,       // critical conditions
    LogErr = LOG_ERR,         // error conditions
    LogWarning = LOG_WARNING, // warning conditions
    LogNotice = LOG_NOTICE,   // normal, but significant, condition
    LogInfo = LOG_INFO,       // informational message
    LogDebug = LOG_DEBUG      // debug-level message
};

std::ostream &operator<<(std::ostream &os, const LogPriority &log_priority);

class Log : public std::basic_streambuf<char, std::char_traits<char>>
{
public:
    explicit Log(std::string ident, int default_priority = LogInfo);
    virtual ~Log();
    
protected:
    int sync();
    int overflow(int c);

private:
    std::string buffer;
    std::string ident;
    int facility;
    int default_priority;
    int log_priority;

    friend std::ostream &operator<<(std::ostream &os, const LogPriority &log_priority);
};

#endif //__LOG_HPP__

/** @} */