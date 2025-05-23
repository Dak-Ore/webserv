#pragma once

#include "Adress.hpp"
#include "Socket.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

#include <string>

class HttpClient : public Socket
{
private:
	Adress _serverAdress;
	HttpRequest request;
	HttpResponse response;;
public:
	HttpClient();
	HttpClient(HttpClient const &ref);
	HttpClient &operator=(HttpClient const &ref);
	~HttpClient();

	HttpClient(int serverFd);
	const Adress &getServerAdress() const;
	bool readRequest();
	void send();
};
