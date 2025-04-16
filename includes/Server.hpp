#pragma once

#include "Socket.hpp"
#include "EPoll.hpp"

#include <vector>
#include <string>

class Server
{
private:
	std::vector<Socket*> _sockets;
	EPoll _epoll;
	void init(std::string hostname, std::string service);
	bool isServerSocket(int fd);
	void acceptClient(int serverFd);
public:
	Server();
	Server(std::string hostname, std::string service);
	~Server();
	void listen();
	std::string readRequest(int fd);
};