/**
 * @file signals.cpp
 * @author Mateus Lima Alves (mateuslima.ti@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-02-16
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <signals.hpp>
#include <exceptions.hpp>
#include <iostream>

/**
 * @brief Construct a new Signals:: Signals object
 *
 */
Signals::Signals(std::list<int> acceptable_signals,
                 std::list<int> lockable_signals,
                 void (*signals_handler_cb)(int signo, siginfo_t *siginfo,
                                            void *context)) : acceptable_signals(acceptable_signals),
                                                              lockable_signals(lockable_signals),
                                                              signals_handler_cb(signals_handler_cb)
{
}

/**
 * @brief Destroy the Signals:: Signals object
 *
 */
Signals::~Signals()
{
}

/**
 * @brief
 *
 */
void Signals::signals_config()
{
    struct sigaction sigaction_set;
    sigset_t blocked_set;

    if (this->signals_handler_cb == nullptr)
    {
        throw Exception(EXCEPTION_MSG("Signals - Signals handler callback is nullptr."));
    }

    sigemptyset(&sigaction_set.sa_mask);

    /** Send all signals to default handler */
    sigaction_set.sa_flags = 0;
    sigaction_set.sa_handler = SIG_DFL;

    for (auto index = 0; index < NSIG; index++)
    {
        sigaction(index, &sigaction_set, NULL);
    }

    /** Send some signals to signals_handler_cb */
    sigaction_set.sa_flags = SA_SIGINFO;
    sigaction_set.sa_sigaction = this->signals_handler_cb;
    for (std::list<int>::iterator it = this->acceptable_signals.begin();
         it != this->acceptable_signals.end(); ++it)
    {
        sigaddset(&sigaction_set.sa_mask, *it);
        sigaction(*it, &sigaction_set, NULL);
    }

    sigemptyset(&blocked_set);
    for (std::list<int>::iterator it = this->lockable_signals.begin();
         it != this->lockable_signals.end(); ++it)
    {
        sigaddset(&blocked_set, *it);
    }

    sigprocmask(SIG_SETMASK, &blocked_set, (sigset_t *)NULL);
}