#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <cstring>
#include "connect.h"
#include "parse.h"
#include "commands.h"

int IRCRegistration(int irc_socket);
int MainLoop(int irc_socket);

int main()
{
	int irc_socket = CreateSocket();

	if (irc_socket == -1)
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

int MainLoop(int irc_socket)
{
	int resultCode = 0, recvbuflen = 512;
	char recvbuf[512];
	std::string display_buffer;
	do {
		resultCode = recv(irc_socket, recvbuf, recvbuflen, 0);
		display_buffer = std::string(recvbuf);
		if (resultCode > 0)
		{
		    std::cout << display_buffer << '\n';	
			IRCMessage filled_message = ParseMessage(display_buffer);
			CheckPingStatus(irc_socket, filled_message);
			CheckCommands(display_buffer, irc_socket, filled_message);
			memset(&recvbuf, 0, sizeof(recvbuf));
		}
		else if (resultCode == 0)
		{
			std::cout << "Connection closed\n";
		}
		else
			std::cout << "recv failed\n";
	} while (resultCode > 0);
	return 0;
}

int IRCRegistration(int irc_socket)
{
    if (SendToServer(irc_socket, "PASS PASSWORD\r\n") == 1)
    {
        return 1;
    }    

    if (SendToServer(irc_socket, "NICK GreenBot\r\n") == 1)
    {
        return 1;
    }    

    if (SendToServer(irc_socket, "USER GreenBot localhost servername :GreenBot\r\n") == 1)
    {
        return 1;
    }    

    if (SendToServer(irc_socket, "JOIN #greenbot\r\n") == 1)
    {
        return 1;
    }    

	return 0;
}
