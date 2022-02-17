/**
 * @file handlers.cpp
 * @author Mateus Lima Alves (mateuslima.ti@gmail.com)
 * @brief
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

volatile int run_service = 1;

/**
 * @brief
 *
 * @param fd
 * @param ac
 */
void process_handler(int fd, std::unique_ptr<std::list<int>> &ac, void *context)
{
    ssize_t count;
    char buf[512];
    int ret;

    count = read(fd, buf, sizeof(buf));
    Counter* counter = static_cast<Counter *>(context);
    std::cout << LogNotice << "Counter " << counter->get_value() << std::endl;
    counter+=10;
    std::cout << LogNotice << "Counter " << counter->get_value() << std::endl;


    if (count > 1)
    {
        for (std::list<int>::iterator it = ac->begin();
             it != ac->end(); ++it)
        {
            ret = write(*it, buf, count);
            if (ret == -1)
            {
                std::cout << LogErr << "write error " << std::endl;
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
    }

    std::cout << "Closed connection on descriptor " << fd << std::endl;
}

/**
 * @brief
 *
 * @param signo
 * @param siginfo
 * @param context
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