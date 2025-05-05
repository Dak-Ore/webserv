#pragma once

#include "utils.hpp"
#include "Adress.hpp"
#include "Socket.hpp"

#include <string>

class HttpClient : public Socket
{
public:
	HttpClient();
	HttpClient(HttpClient const &ref);
	HttpClient &operator=(HttpClient const &ref);
	~HttpClient();

	HttpClient(int serverFd);
};
