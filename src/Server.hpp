#pragma once

#include "Socket.hpp"

#include <vector>
#include <string>

class Server
{
private:
	//int _epollFd;
	std::vector<Socket*> sockets;
	void init(std::string hostname, std::string service);
public:
	Server();
	Server(std::string hostname, std::string service);
	void listen();
	std::string readRequest(int fd);
	~Server();
};