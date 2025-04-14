#pragma once

#include <sys/socket.h>
#include <string>

class Socket
{
private:
	int _fd;
	void init(std::string hostname, std::string service);
public:
	Socket();
	Socket(std::string hostname, std::string service);
	~Socket();

	int getFd();
	void listen(int backlog = SOMAXCONN);
};
