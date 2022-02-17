/**
 * @file signals.hpp
 * @author Mateus Lima Alves (mateuslima.ti@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-02-16
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __SIGNALS_HPP__
#define __SIGNALS_HPP__

#include <signal.h>
#include <list>

class Signals
{
private:
    void (*signals_handler_cb)(int signo, siginfo_t *siginfo,
                               void *context);
    std::list<int> acceptable_signals;
    std::list<int> lockable_signals;

public:
    Signals(std::list<int> acceptable_signals = {},
            std::list<int> lockable_signals = {},
            void (*signals_handler_cb)(int signo, siginfo_t *siginfo,
                                       void *context) = nullptr);
    ~Signals();

    void signals_config();
};

#endif //__SIGNALS_HPP__