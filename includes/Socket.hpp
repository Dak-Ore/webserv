#pragma once

#include <sys/socket.h>
#include <string>

class Socket
{
private:
	int _fd;
public:
	Socket(std::string hostname = "localhost", std::string service = "80");
	~Socket();

	int getFd();
	void listen(int backlog = SOMAXCONN);
};
