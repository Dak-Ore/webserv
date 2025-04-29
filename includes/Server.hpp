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
	EPoll &_epoll;
    std::string _root;
public:
	ServerConfig	_config;
	Server(EPoll &epoll_ref, ServerConfig &config);
	~Server();
	HttpRequest readRequest(int fd);
	bool handleRequest(HttpRequest const &request, int response_fd);
	std::string getIndex(std::string root, std::string path);
};