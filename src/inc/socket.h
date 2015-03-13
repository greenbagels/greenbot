#ifndef _SOCKET_H
#define _SOCKET_H

#define PACKET_SIZE 512 // For recv calls.

class Socket {
  private:
    int socketFd;
  public:
    void Send(std::string message);
    std::string Recv();
    Socket(std::string server, std::string port);
};

#endif /* _SOCKET_H */

