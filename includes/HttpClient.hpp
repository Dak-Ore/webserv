#pragma once

#include "utils.hpp"
#include <string>

class HttpClient
{
private:
	int _fd;
	int _host;
	int _port;

public:
	HttpClient();
	HttpClient(int serverFd);
	HttpClient(HttpClient const &ref);
	HttpClient &operator=(HttpClient const &ref);
	~HttpClient();
	int getFd() const;
	int getHost() const;
	int getPort() const;
};
