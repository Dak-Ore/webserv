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
    std::map<int, HttpClient> _clients;
    std::map<int, CGI::Running&> _CGIs;
    std::vector<Server*> _servers;
	std::vector<ServerSocket> _sockets;
	bool isServerSocket(int fd);
	bool isClientSocket(int fd);
	Server& findServer(const HttpRequest  &request, const HttpClient &client) const;
	void acceptClient(int serverFd);
	void removeClient(int fd);
	void removeClient(const HttpClient &client);
	bool handleRedirect(HttpResponse &response, LocationConfig *location);
	void sendAutoindex(const HttpRequest &request, HttpResponse &response, const std::string &directory);
	void handleErrorPages(HttpResponse &response, Config *config);
	void handleRequest(HttpClient &client);
	bool handleCgi(HttpClient &client, LocationConfig *location, std::string file_path);
	void timeout();
public:
    Webserv(ConfigParser &parser);
    ~Webserv();
	Config* findConfig(const HttpClient &client) const;
    void listen();
    void stop();
};
