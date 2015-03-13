// @file socket_posix.cpp
// @brief Implementation of the Socket class.
//
// Note: Only POSIX operating systems can compile this file.
//

#ifdef __unix__

#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <iostream> // std::string
#include <cstring>  // memset

#include "socket.h"

void
Socket::Send(std::string message)
{
  if (send(socketFd, message.c_str(), message.length(), 0) == -1)
  {
      // TODO: Proper exception.
      std::cout << "Send failed!" << std::endl;
      close(socketFd);
      return;
  }
}

std::string
Socket::Recv()
{
  char recvbuf[PACKET_SIZE];
  memset(&recvbuf, 0, sizeof(recvbuf));
  int resultCode = recv(socketFd, recvbuf, PACKET_SIZE, 0);
  std::string out;
  if (resultCode > 0)
  {
    out = std::string(recvbuf);
  }
  else if (resultCode == 0)
  {
		std::cout << "Connection closed" << std::endl;
  }
  else
  {
		std::cout << "Recv failed" << std::endl;
  }
  return out;
}

Socket::Socket(std::string server, std::string port)
{
  struct addrinfo host, *host_list;

  memset(&host, 0, sizeof(host));
  host.ai_family = AF_UNSPEC;
  host.ai_socktype = SOCK_STREAM;
  host.ai_protocol = IPPROTO_TCP;

  int result = getaddrinfo(server.c_str(), port.c_str(), &host, &host_list);
  if (result != 0)
  {
    std::cout << "getaddrinfo failed! " << gai_strerror(result);
  }
  int connected_socket = -1;
  connected_socket = socket(host_list->ai_family, host_list->ai_socktype, host_list->ai_protocol);

  if (connected_socket == -1)
  {
    std::cout << "Error at socket()!\n";
    freeaddrinfo(host_list);
    return;
  }

  result = connect(connected_socket, host_list->ai_addr, host_list->ai_addrlen);
  if (result == -1)
  {
    result = connect(connected_socket, host_list->ai_next->ai_addr, host_list->ai_next->ai_addrlen);
    if (result == -1)
    {
      close(connected_socket);
      connected_socket = -1;
      std::cout << "Unable to connect to server!\n";
      return;
    }
  }

	socketFd = connected_socket;
}

#endif /* __unix__ */
