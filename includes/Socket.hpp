#pragma once

#include <sys/socket.h>
#include <string>
#include <unistd.h>

class Socket
{
private:
	int _fd;
	int _host;
	int _port;
	void setSocketInfo();

public:
	static std::pair<int, int>getSocketInfo(int fd);
	Socket(std::string hostname = "localhost", std::string service = "80");
	~Socket();

	int getFd();
	void listen();
};
