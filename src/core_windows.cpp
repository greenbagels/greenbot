#include <WinSock2.h>
#include <Windows.h>
#include <WS2tcpip.h>
#include <iostream>
#include <string>
#include "connect.h"
#include "parse.h"
#include "commands.h"

int IRCRegistration(SOCKET irc_socket);
int MainLoop(SOCKET irc_socket);

int main()
{
	SOCKET irc_socket = CreateSocket();

	if (irc_socket == INVALID_SOCKET)
	{
		return 1;
	}

	if (IRCRegistration(irc_socket))
	{
		return 1;
	}
	MainLoop(irc_socket);
	return 0;
}

int MainLoop(SOCKET irc_socket)
{
	int result_code = 0, recvbuflen = 512;
	char recvbuf[512];
	std::string display_buffer;
	bool serverflag = false;
	do {
		result_code = recv(irc_socket, recvbuf, recvbuflen, 0);
		display_buffer = std::string(recvbuf);
		if (result_code > 0)
		{
            std::cout << display_buffer << '\n';
			IRCMessage filled_message = ParseMessage(display_buffer);
			CheckPingStatus(display_buffer, irc_socket, filled_message);
			CheckCommands(display_buffer, irc_socket, filled_message);
			ZeroMemory(&recvbuf, sizeof(recvbuf));
		}
		else if (result_code == 0)
		{
			std::cout << "Connection closed\n";
		}
		else
			std::cout << "recv failed " << WSAGetLastError() << '\n';
	} while (result_code > 0);
	return 0;
}

int IRCRegistration(SOCKET irc_socket)
{
    if (SendToServer(irc_socket, "PASS PASSWORD\r\n") == 1)
	{
		return 1;
	}

	if (SendToServer(irc_socket, "NICK GreenBot\r\n") == 1)
	{
		return 1;
	}

	if (SendToServer(irc_socket, "USER GreenBot localhost servername :GreenBot\r\n" == 1)
	{
	    return 1;
	}

	if (SendToServer(irc_socket, "JOIN #greenbot\r\n" == 1)
	{
		return 1;
	}
	
    return 0;
}

