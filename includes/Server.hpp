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
    std::string _root;
public:
	ServerConfig	_config;
	Server(ServerConfig &config);
	~Server();
	bool handleRequest(HttpRequest const &request, const HttpClient &client);
	std::string findIndex(const std::string& path, const LocationConfig* location);
	const LocationConfig *matchLocation(const HttpRequest& request);
};