/**
 * @file log.cpp
 * @author Mateus Lima (mateuslima.ti@gmail.com)
 * @brief Log class methods.
 * @version 0.1
 * @date 2022-02-15
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <cstring>
#include <ostream>
#include <log.hpp>

/**
 * @brief Construct a new Log:: Log object
 *
 * @param ident String for log identity
 */
Log::Log(std::string ident, int default_priority):
    ident(ident),
    default_priority(default_priority)
{
    openlog(this->ident.c_str(), LOG_PID, LOG_DAEMON);
}

/**
 * @brief Destroy the Log:: Log object
 * 
 */
Log::~Log()
{
    closelog();
}

/**
 * @brief Sync method
 *
 * @return void
 */
int Log::sync(void)
{
    if (this->buffer.length())
    {
        syslog(this->log_priority, "%s", this->buffer.c_str());
        this->buffer.erase();
        this->log_priority = this->default_priority; // Return priority_ to default value
    }
    return 0;
}

/**
 * @brief Overflow
 *
 * @param c
 * @return int
 */
int Log::overflow(int c)
{
    if (c != EOF)
    {
        this->buffer += static_cast<char>(c);
    }
    else
    {
        sync();
    }
    return c;
}

/**
 * @brief
 *
 * @param os
 * @param log_priority
 * @return std::ostream&
 */
std::ostream &operator<<(std::ostream &os, const LogPriority &log_priority)
{
    static_cast<Log *>(os.rdbuf())->log_priority = (int)log_priority;
    return os;
}