#pragma once

#include "ConfigParser.hpp"
#include "Server.hpp"
#include "EPoll.hpp"
#include "HttpClient.hpp"
#include "ServerSocket.hpp"

#include <vector>
#include <map>

class HttpClient;
class HttpRequest;

class Webserv
{
private:
    bool _run;
    EPoll _epoll;
    std::map<int, HttpClient> _client_map;
    std::vector<Server*> _servers;
	std::vector<ServerSocket> _sockets;
	bool isServerSocket(int fd);
	bool isClientSocket(int fd);
	Server& findServer(const HttpRequest  &request, const HttpClient &client) const;
	void acceptClient(int serverFd);
public:
    Webserv(ConfigParser &parser);
    ~Webserv();
	Config* findConfig(const HttpRequest  &request, const HttpClient &client) const;
    void listen();
    void stop();
};
