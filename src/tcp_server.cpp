/**
 * @file tcp_server.cpp
 * @author Mateus Alves (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-02-15
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include <tcp_server.hpp>
#include <exceptions.hpp>
#include <iostream>
#include <log.hpp>

/**
 * @brief Construct a new Tcp Server:: Tcp Server object
 *
 * @param port
 */
TcpServer::TcpServer(std::string port, void (*process_event_cb)(int fd)) : port(port),
                                                                           process_event_cb(process_event_cb)
{
}

/**
 * @brief Destroy the Tcp Server:: Tcp Server object
 *
 */
TcpServer::~TcpServer()
{
    std::cout << "Destructor" << std::endl;
    free(this->epoll_events);
    close(this->s_file_descriptor);
}

/**
 * @brief
 *
 * @return int
 */
int TcpServer::socket_bind()
{
    int ret, fd, option;
    struct addrinfo hints, *socket_address;

    memset(&hints, 0, sizeof(addrinfo));
    hints.ai_family = AF_UNSPEC;     /* Return IPv4 and IPv6 choices */
    hints.ai_socktype = SOCK_STREAM; /* We want a TCP socket */
    hints.ai_flags = AI_PASSIVE;     /* All interfaces */

    ret = getaddrinfo(NULL, this->port.c_str(), &hints, &socket_address);
    if (ret != 0)
    {
        throw Exception(EXCEPTION_MSG("Socket - getaddrinfo error."));
    }

    for (; socket_address != NULL; socket_address = socket_address->ai_next)
    {
        fd = socket(socket_address->ai_family, socket_address->ai_socktype, socket_address->ai_protocol);
        if (fd == -1)
        {
            continue;
        }

        ret = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
        if (ret == -1)
        {
            throw Exception(EXCEPTION_MSG("Socket - Could not set socket options."));
        }

        ret = bind(fd, socket_address->ai_addr, socket_address->ai_addrlen);
        if (ret == 0)
        {
            /* We managed to bind successfully! */
            break;
        }
        close(fd);
    }
    if (socket_address == NULL)
    {
        throw Exception(EXCEPTION_MSG("Socket - Could not bind."));
    }

    freeaddrinfo(socket_address);
    this->make_non_blocking(fd);

    this->s_file_descriptor = fd;

    return fd;
}

/**
 * @brief
 *
 */
void TcpServer::make_non_blocking(int file_descriptor)
{
    int flags, ret;
    flags = fcntl(file_descriptor, F_GETFL, 0);
    if (flags == -1)
    {
        throw Exception(EXCEPTION_MSG("Socket - Could not get file descriptor flags."));
    }
    flags |= O_NONBLOCK;
    ret = fcntl(file_descriptor, F_SETFL, flags);
    if (ret == -1)
    {
        throw Exception(EXCEPTION_MSG("Socket - Could not set file descriptor flags."));
    }
}

/**
 * @brief
 *
 */
void TcpServer::socket_listen()
{
    int ret, epoll_file_descriptor;
    struct epoll_event event;

    ret = listen(this->s_file_descriptor, TCP_SERVER_MAX_CONNECTIONS);
    if (ret == -1)
    {
        throw Exception(EXCEPTION_MSG("Socket - Could not listen."));
    }
    this->e_file_descriptor = epoll_create1(0);
    if (this->e_file_descriptor == -1)
    {
        throw Exception(EXCEPTION_MSG("Socket - Could not created epoll."));
    }

    event.data.fd = this->s_file_descriptor;
    event.events = EPOLLIN | EPOLLET;
    ret = epoll_ctl(this->e_file_descriptor, EPOLL_CTL_ADD, this->s_file_descriptor, &event);
    if (ret == -1)
    {
        throw Exception(EXCEPTION_MSG("Socket - Could not manipulate an epoll instance."));
    }

    /* Buffer where events are returned */
    this->epoll_events = (epoll_event *)calloc(TCP_SERVER_MAX_EVENTS, sizeof(event));
}

/**
 * @brief
 *
 */
void TcpServer::process_events()
{
    int n_events, ret;
    struct epoll_event event;

    n_events = epoll_wait(this->e_file_descriptor, this->epoll_events, TCP_SERVER_MAX_EVENTS, TCP_SERVER_WAIT_TIMEOUT);
    for (int idx_event = 0; idx_event < n_events; idx_event++)
    {
        if ((this->epoll_events[idx_event].events & EPOLLERR) ||
            (this->epoll_events[idx_event].events & EPOLLHUP) ||
            (!(this->epoll_events[idx_event].events & EPOLLIN)))
        {
            /* An error has occured on this fd, or the socket is not
                ready for reading (why were we notified then?) */
            std::cout << LogErr << "epoll error. Code " << this->epoll_events[idx_event].events << std::endl;
            close(this->epoll_events[idx_event].data.fd);
            continue;
        }

        else if (this->s_file_descriptor == this->epoll_events[idx_event].data.fd)
        {
            /* We have a notification on the listening socket, which
                means one or more incoming connections. */
            while (1)
            {
                struct sockaddr in_addr;
                socklen_t in_len;
                int in_file_descriptor;

                in_len = sizeof(in_addr);
                in_file_descriptor = accept(this->s_file_descriptor, &in_addr, &in_len);
                if (in_file_descriptor == -1)
                {
                    if ((errno == EAGAIN) ||
                        (errno == EWOULDBLOCK))
                    {
                        /* We have processed all incoming
                            connections. */
                        break;
                    }
                    else
                    {
                        std::cout << LogErr << "Could not accept conection." << std::endl;
                        break;
                    }
                }

                std::cout << "Accepted connection on descriptor " << in_file_descriptor << std::endl;

                /* Make the incoming socket non-blocking and add it to the
                    list of fds to monitor. */
                this->make_non_blocking(in_file_descriptor);

                event.data.fd = in_file_descriptor;
                event.events = EPOLLIN | EPOLLET;
                ret = epoll_ctl(this->e_file_descriptor, EPOLL_CTL_ADD, in_file_descriptor, &event);
                if (ret == -1)
                {
                    std::cout << LogErr << "epoll error " << std::endl;
                }
            }
            continue;
        }
        else
        {
            if (this->process_event_cb == nullptr)
            {
                std::cout << LogErr << "Process event callback is null " << std::endl;
            }
            else
            {
                this->process_event_cb(this->epoll_events[idx_event].data.fd);
            }

            close(this->epoll_events[idx_event].data.fd);
        }
    }
}