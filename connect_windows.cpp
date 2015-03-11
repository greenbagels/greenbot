#include <winsock2.h>
#include <Ws2tcpip.h>
#include <iostream>
#include <cstring>
#include "connect.h"
#include "parse.h"

#pragma comment(lib, "Ws2_32.lib");

int SendToServer(int irc_socket, const char* message)
{
    if (send(irc_socket, message, strlen(message), NULL) == SOCKET_ERROR)
    {
        std::cout << "send failed: " << WSAGetLastError() << '\n';
        closesocket(irc_socket);
        WSACleanup();
        return 1;
    }
    return 0;
}

int CheckPingStatus(int irc_socket, IRCMessage filled_message)
{
	if (filled_message.command == "PING")
	{
		std::string pong = std::string("PONG :") + filled_message.trail + std::string("\r\n");
		std::cout << "LOCAL: Sent string: " << pong;
	    return SendToServer(irc_socket, pong.c_str());
    }
	return 0;
}

int CreateSocket()
{
	WSADATA wsa_data;

	int result_code = WSAStartup(MAKEWORD(2, 2), &wsa_data); //Specifies version 2.2, by passing 00100010 (34)

	if (result_code != 0)
	{
		std::cout << "WSAStartup Failed!: " << result_code << '\n';
		return INVALID_SOCKET;
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
		return INVALID_SOCKET;
	}
	int connect_socket = INVALID_SOCKET;
	ptr = result;
	connect_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

	if (connect_socket == INVALID_SOCKET)
	{
		std::cout << "Error at socket()! " << WSAGetLastError() << '\n';
		freeaddrinfo(result);
		WSACleanup();
		return connect_socket;
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
	return connect_socket;
}
