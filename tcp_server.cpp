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
#include <tcp_server.hpp>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <exceptions.hpp>
#include <unistd.h>

/**
 * @brief Construct a new Tcp Server:: Tcp Server object
 * 
 * @param port 
 */
TcpServer::TcpServer(std::string port) : 
    port(port)
{
}

/**
 * @brief Destroy the Tcp Server:: Tcp Server object
 * 
 */
TcpServer::~TcpServer()
{
}

/**
 * @brief 
 * 
 * @return int 
 */
int TcpServer::socket_bind()
{
    int ret, fd;
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
            continue;

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
    this->file_descriptor = fd;

    return fd;
}