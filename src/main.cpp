/**
 * @file main.cpp
 * @author Mateus Lima (mateuslima.ti@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-02-15
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <iostream>
#include <log.hpp>
#include <exceptions.hpp>
#include <tcp_server.hpp>
#include <counter.hpp>
#include <signals.hpp>
#include <main.hpp>
#include <handlers.hpp>

/**
 * @brief
 *
 * @return int
 */
int main()
{

    std::cout.rdbuf(new Log(SERVICE_NAME));
    std::cout << "Service started." << std::endl;

    try
    {
        std::shared_ptr<Counter> counter = std::make_shared<Counter>();

        std::unique_ptr<Signals> signals(new Signals({SIGTERM}, {}, &signals_handler));
        signals->signals_config();

        std::unique_ptr<TcpServer> tcpServer(new TcpServer(SERVICE_PORT, &process_handler, &counter));
        tcpServer->socket_bind();

        tcpServer->socket_listen();
        while (run_service)
        {
            tcpServer->process_events();
        };
    }
    catch (const std::exception &e)
    {
        std::cerr << LogErr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}