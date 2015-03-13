// @file socket_windows.cpp
// @brief Implementation of the Socket class.
//
// Note: Only Windows operating systems can compile this file.
//

#ifdef _WIN32

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <iostream>
#include <cstring>
#include "connect.h"
#include "parse.h"

#pragma comment(lib, "Ws2_32.lib");

#include "socket.h"

  void
Socket::Send(std::string message)
{
  if (send(irc_socket, message, strlen(message), NULL) == SOCKET_ERROR)
  {
    std::cout << "send failed: " << WSAGetLastError() << '\n';
    closesocket(irc_socket);
    WSACleanup();
  }
}

  std::string
Socket::Recv()
{
  char recvbuf[PACKET_SIZE];
  ZeroMemory(&recvbuf, sizeof(recvbuf));
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
  WSADATA wsa_data;

  int result_code = WSAStartup(MAKEWORD(2, 2), &wsa_data); //Specifies version 2.2, by passing 00100010 (34)

  if (result_code != 0)
  {
    std::cout << "WSAStartup Failed!: " << result_code << '\n';
    return;
  }
  struct addrinfo *result = NULL, *ptr = NULL, hints;

  ZeroMemory(&hints, sizeof(hints));

  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  result_code = getaddrinfo("irc.rizon.net", "6667", &hints, &result);
  if (result_code != 0)
  {
    std::cout << "getaddrinfo failed! " << result_code << '\n';
    WSACleanup();
    return;
  }
  int connect_socket = INVALID_SOCKET;
  ptr = result;
  connect_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

  if (connect_socket == INVALID_SOCKET)
  {
    std::cout << "Error at socket()! " << WSAGetLastError() << '\n';
    freeaddrinfo(result);
    WSACleanup();
    return;
  }

  result_code = connect(connect_socket, ptr->ai_addr, static_cast<int>(ptr->ai_addrlen));
  if (result_code == SOCKET_ERROR)
  {
    result_code = connect(connect_socket, ptr->ai_next->ai_addr, static_cast<int>(ptr->ai_next->ai_addrlen));
    if (result_code == SOCKET_ERROR)
    {
      closesocket(connect_socket);
      connect_socket = INVALID_SOCKET;
    }
  }

  freeaddrinfo(result);

  if (connect_socket == INVALID_SOCKET)
  {
    std::cout << "Unable to connect to server!\n";
    WSACleanup();
  }

  socketFd = connect_socket;
}

#endif /* _WIN32 */

