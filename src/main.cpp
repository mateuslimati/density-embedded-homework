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
#include <unistd.h>
#include <log.hpp>
#include <exceptions.hpp>
#include <tcp_server.hpp>
#include <memory>
using namespace std;

void process_handler(int fd)
{
  ssize_t count;
  char buf[512];
  int ret;

  count = read(fd, buf, sizeof(buf));
  if (count > 1)
  {
    /* Write the buffer to standard output */
    ret = write(fd, buf, count);
    if (ret == -1)
    {
      std::cout << LogErr << "write error " << std::endl;
    }
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
 * @return int
 */
int main()
{

  cout.rdbuf(new Log("counter-service"));

  try
  {
    unique_ptr<TcpServer> tcpServer(new TcpServer("8089", &process_handler));
    tcpServer->socket_bind();
    tcpServer->socket_listen();
    cout << "TCP Server created" << endl;
    while (1)
    {
      tcpServer->process_events();
    };
  }
  catch (const std::exception &e)
  {
    cerr << LogErr << e.what() << endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}