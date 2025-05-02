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
	bool handleRequest(HttpRequest const &request, int response_fd);
	std::string findIndex(const std::string& path, const LocationConfig* location);
	const LocationConfig *matchLocation(const HttpRequest& request);
};