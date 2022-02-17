/**
 * @file main.cpp
 * @author Mateus Lima (mateuslima.ti@gmail.com)
 * @brief Service main file
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
 * @brief service main function.
 * 
 * @param[in] argc argument count
 * @param[in] argv argument vector
 * @return int 
 */
int main(int argc, char *argv[])
{
    /** Redirect cout to syslog using Log class */
    std::cout.rdbuf(new Log(SERVICE_NAME));
    std::cout << "Service started." << std::endl;

    try
    {
        /** Make a Counter shared instance */
        std::shared_ptr<Counter> counter = std::make_shared<Counter>();
        /** Make a Signals instance, passing acceptable signals and signals handler callback */
        std::unique_ptr<Signals> signals(new Signals({SIGTERM}, {}, &signals_handler));
        signals->signals_config();

        /** Make a TcpServer instance, passing socket port, process handler callback, and counter pointer */
        std::unique_ptr<TcpServer> tcpServer(new TcpServer(SERVICE_PORT, &process_handler, &counter));
        tcpServer->socket_bind();
        tcpServer->socket_listen();
        
        while (run_service)
        {
            /** handles socket events */
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