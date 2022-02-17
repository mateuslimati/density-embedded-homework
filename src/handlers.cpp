/**
 * @file handlers.cpp
 * @author Mateus Lima Alves (mateuslima.ti@gmail.com)
 * @brief Handler methods.
 * @version 0.1
 * @date 2022-02-17
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <handlers.hpp>
#include <log.hpp>
#include <iostream>
#include <unistd.h>
#include <counter.hpp>
#include <string.h>
#include <algorithm>

volatile int run_service = 1;

/**
 * @brief Funcion that verifing if a string contents a number.
 *
 * @param[in] s string that will be verify
 * @return      true if contents a number
 * @return      false if not contents
 */
bool is_number(const std::string &s)
{
    return !s.empty() && std::find_if(s.begin(),
                                      s.end(), [](unsigned char c)
                                      { return !std::isdigit(c); }) == s.end();
}

/**
 * @brief Function that processing a buffer that can contents a message.
 *
 * @param[in] buffer    char array that will be verify
 * @param[out] counter  smart pointer that manager the shared counter
 * @return int          process status (PROCESS_UNKNOWN, PROCESS_INCR, PROCESS_DECR, PROCESS_OUTPUT)
 */
int process_message(char *buffer, std::shared_ptr<Counter> *counter)
{
    std::string s_buff(buffer);
    std::string delimiter = std::string(" ");
    int founded_delimiter = 0;
    int process_code = PROCESS_UNKNOWN;
    size_t pos = 0;
    std::string token;

    if (s_buff.find(CTRL_STRING) != (s_buff.size() - 2))
    {
        return PROCESS_UNKNOWN;
    }

    s_buff.erase(s_buff.size() - 2, s_buff.size());

    while ((pos = s_buff.find(delimiter)) != std::string::npos)
    {
        token = s_buff.substr(0, pos);
        s_buff.erase(0, pos + delimiter.length());
        founded_delimiter += 1;
        if (token.compare(INCR_STRING) == 0)
        {
            process_code = PROCESS_INCR;
        }
        if (token.compare(DECR_STRING) == 0)
        {
            process_code = PROCESS_DECR;
        }
        if (founded_delimiter > 1)
        {
            return PROCESS_UNKNOWN;
        }
    }
    if (founded_delimiter == 0)
    {
        if (s_buff.compare(OUTPUT_STRING) == 0)
        {
            return PROCESS_OUTPUT;
        }
        return PROCESS_UNKNOWN;
    }
    if (is_number(s_buff) == false)
    {
        return PROCESS_UNKNOWN;
    }
    switch (process_code)
    {
    case PROCESS_INCR:
        *(*counter) += stoi(s_buff);
        break;
    case PROCESS_DECR:
        *(*counter) -= stoi(s_buff);
        break;
    default:
        break;
    }
    return process_code;
}

/**
 * @brief Callback used to processing a request from a file descriptor
 *
 * @param[in] fd        file descriptor that requested something
 * @param[in] ac        active connections
 * @param[out] context  void point that contents the shared counter
 */
void process_handler(int fd, std::unique_ptr<std::list<int>> &ac, void *context)
{
    ssize_t count;
    char buf[512] = {0};
    int ret, value, process_code = PROCESS_UNKNOWN;
    std::string s_counter;

    count = read(fd, buf, sizeof(buf));
    std::shared_ptr<Counter> *counter = static_cast<std::shared_ptr<Counter> *>(context);

    if (count > 1)
    {
        if (process_code = process_message(buf, counter))
        {
            switch (process_code)
            {
            case PROCESS_OUTPUT:
                s_counter = std::to_string((*counter)->get_value()) + CTRL_STRING;
                ret = write(fd, s_counter.c_str(), s_counter.size());
                if (ret == -1)
                {
                    std::cout << LogErr << "write error " << std::endl;
                }
                break;
            case PROCESS_UNKNOWN:
                break;
            default:
                s_counter = std::to_string((*counter)->get_value()) + CTRL_STRING;
                for (std::list<int>::iterator it = ac->begin();
                     it != ac->end(); ++it)
                {
                    ret = write(*it, s_counter.c_str(), s_counter.size());
                    if (ret == -1)
                    {
                        std::cout << LogErr << "write error " << std::endl;
                    }
                }
                break;
            }
        }
        /* Write the buffer to standard output */
    }
    else if (count == -1)
    {
        /* If errno == EAGAIN, that means we have read all
            data. So go back to the main loop. */
        if (errno != EAGAIN)
        {
            std::cout << LogErr << "read error " << std::endl;
        }
    }
    else
    {
        std::cout << LogNotice << "The remote has closed the connection " << fd << std::endl;
        /* End of file. The remote has closed the
            connection. */
        close(fd);
        ac->remove(fd);
    }
}

/**
 * @brief Callback used to processing signals handler. See sigaction man.
 *
 * @param[in] signo
 * @param[in] siginfo
 * @param[in] context
 */
void signals_handler(int signo, siginfo_t *siginfo, void *context)
{
    std::cout << LogNotice << "Signal received " << signo << std::endl;
    switch (signo)
    {
    case SIGTERM:
        std::cout << LogNotice << "SIGTERM received, stopping service..." << signo << std::endl;
        run_service = 0;
        break;

    default:
        break;
    }
}