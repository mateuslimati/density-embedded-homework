/**
 * @file handlers.hpp
 * @author Mateus Lima Alves (mateuslima.ti@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-02-17
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __HANDLERS_HPP__
#define __HANDLERS_HPP__

#include <memory>
#include <list>
#include <signals.hpp>
#include <counter.hpp>

extern volatile int run_service;

void process_handler(int fd, std::unique_ptr<std::list<int>> &ac, void *context);
void signals_handler(int signo, siginfo_t *siginfo, void *context);

#endif //__HANDLERS_HPP__