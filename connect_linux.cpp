#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include "connect.h"
#include "parse.h"

int SendToServer(int irc_socket, const char* message)
{
    if (send(irc_socket, message, strlen(message), 0) == -1)
    {
        std::cout << "send failed!\n";
        close(irc_socket);
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
	struct addrinfo host, *host_list;

	memset(&host, 0, sizeof(host));
	host.ai_family = AF_UNSPEC;
	host.ai_socktype = SOCK_STREAM;
	host.ai_protocol = IPPROTO_TCP;

	int result = getaddrinfo("irc.rizon.net", "6667", &host, &host_list);
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
		return -1;
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
        }
	}
	return connected_socket;
}
