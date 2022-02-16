/**
 * @file tcp_server.hpp
 * @author Mateus Lima Alves (mateuslima.ti@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-02-15
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __TCP_SERVER_HPP__
#define __TCP_SERVER_HPP__

#include <string>
#include <sys/epoll.h>

#define TCP_SERVER_MAX_CONNECTIONS  1024
#define TCP_SERVER_MAX_EVENTS 64

#define TCP_SERVER_WAIT_TIMEOUT 500 // In milliseconds.

class TcpServer
{
public:
    explicit TcpServer(std::string port, void (* process_event_cb)(int fd) = nullptr);
    virtual ~TcpServer();
    int socket_bind();
    void socket_listen();
    void process_events();

protected:
    void make_non_blocking(int file_descriptor);

private:
    std::string port;
    int s_file_descriptor;
    int e_file_descriptor;
    struct epoll_event *epoll_events;
    void (*process_event_cb)(int fd);  
};

#endif //__TCP_SERVER_HPP__

/** @} */