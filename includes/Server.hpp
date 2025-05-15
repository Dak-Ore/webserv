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
	bool handleRedirect(HttpResponse &response, LocationConfig *location);
	void sendAutoindex(const HttpRequest &request, HttpResponse &response, const std::string &directory);
	void handleErrorPages(HttpResponse &response, Config *config);

public:
	ServerConfig	_config;
	Server(ServerConfig &config);
	~Server();
	bool handleRequest(HttpRequest const &request, const HttpClient &client);
	Config *matchLocation(const HttpRequest& request);
	std::string findIndex(const std::string& path, const LocationConfig* location);
};