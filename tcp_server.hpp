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

class TcpServer
{
public:
    explicit TcpServer(std::string port);
    virtual ~TcpServer();
    int socket_bind();

private:
    std::string port;
    int file_descriptor;
};

#endif //__TCP_SERVER_HPP__

/** @} */