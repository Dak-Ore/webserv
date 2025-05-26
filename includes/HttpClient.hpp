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
	HttpRequest _request;
	HttpResponse _response;;
	Webserv		*_server;
public:
	HttpClient();
	HttpClient(HttpClient const &ref);
	HttpClient &operator=(HttpClient const &ref);
	~HttpClient();

	HttpClient(int serverFd, Webserv &server);
	const Adress &getServerAdress() const;
	HttpRequest &request();
	HttpResponse&response();
	const HttpRequest &request() const;
	const HttpResponse&response() const;
	bool readRequest();
	void send();
};
