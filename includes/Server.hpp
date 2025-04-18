#pragma once

#include "Socket.hpp"
#include "EPoll.hpp"
#include "HttpRequest.hpp"
#include "ServerConfig.hpp"
#include <vector>
#include <string>

class Server
{
private:
	bool					_run;
	std::vector<Socket*>	_sockets;
	EPoll					_epoll;
	ServerConfig			_config;
	bool isServerSocket(int fd);
	void acceptClient(int serverFd);
public:
	Server(ServerConfig config);
	~Server();
	void stop();
	void listen();
	HttpRequest readRequest(int fd);
	bool handleRequest(HttpRequest const &request, int response_fd);
};