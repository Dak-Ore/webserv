#pragma once

#include "utils.hpp"
#include "Adress.hpp"
#include "Socket.hpp"

#include <string>

class HttpClient : public Socket
{
private:
	Adress _serverAdress;
public:
	HttpClient();
	HttpClient(HttpClient const &ref);
	HttpClient &operator=(HttpClient const &ref);
	~HttpClient();

	HttpClient(int serverFd);
	const Adress &getServerAdress() const;
};
