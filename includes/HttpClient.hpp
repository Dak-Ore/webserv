#pragma once

#include "utils.hpp"
#include "Adress.hpp"
#include "Socket.hpp"

#include <string>

class HttpClient : public Socket
{
public:
	HttpClient();
	HttpClient(int serverFd);
	HttpClient(HttpClient const &ref);
	HttpClient &operator=(HttpClient const &ref);
	~HttpClient();
};
