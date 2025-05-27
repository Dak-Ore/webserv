#pragma once

#include "ServerConfig.hpp"
#include <vector>
#include <string>

class HttpRequest;

class Server
{
private:
    std::string _root;

public:
	ServerConfig	_config;
	Server(ServerConfig &config);
	~Server();
	Config *getConfig(const HttpRequest& request);
};