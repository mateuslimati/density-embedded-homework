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
    explicit Log(std::string ident);

protected:
    int sync();
    int overflow(int c);

private:
    std::string buffer;
    int facility;
    int priority;
    char ident[50];

    friend std::ostream &operator<<(std::ostream &os, const LogPriority &log_priority);
};