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
Log::Log(std::string ident)
{
    priority = LogInfo;
    strncpy(this->ident, ident.c_str(), sizeof(this->ident));
    this->ident[sizeof(this->ident) - 1] = '\0';

    openlog(this->ident, LOG_PID, LOG_DAEMON);
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
        syslog(this->priority, "%s", this->buffer.c_str());
        this->buffer.erase();
        this->priority = LogInfo; // Return priority_ to default value
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
    static_cast<Log *>(os.rdbuf())->priority = (int)log_priority;
    return os;
}