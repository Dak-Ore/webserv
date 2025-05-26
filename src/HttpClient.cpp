#include "HttpClient.hpp"
#include "Webserv.hpp"
#include "Socket.hpp"
#include "Adress.hpp"
#include "utils.hpp"
#include <netdb.h>
#include <string>

HttpClient::HttpClient() : Socket(),
	_server(NULL)
{
}

HttpClient::HttpClient(int serverFd, Webserv &server) : Socket(),
	_server(&server)
{
	sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int fd = ::accept(serverFd, (struct sockaddr *)&client_addr, &client_len);
	if (fd == -1)
		return ;
	this->setFd(fd);
	this->_adress = Adress(client_addr);
	this->_serverAdress = Adress(this);
}

HttpClient::HttpClient(HttpClient const &ref) : Socket()
{
	*this = ref;
}
HttpClient &HttpClient::operator=(HttpClient const &ref)
{
	this->setFd(ref.getFd());
	this->_request = ref._request;
	this->_response = ref._response;
	this->_adress = ref._adress;
	this->_serverAdress = ref._serverAdress;
	this->_server = ref._server;
	return (*this);
}

HttpClient::~HttpClient()
{
}

const Adress &HttpClient::getServerAdress() const
{
	return (this->_serverAdress);
}

bool HttpClient::readRequest()
{
	std::string content = this->read();
	this->_request.read(content);
	if (this->_request.getConfig() == NULL && this->_request.isHeaderReady())
	{
		Config *config = this->_server->findConfig(*this);
		this->_request.setConfig(config);
	}
	return (this->_request.isReady());
}

void HttpClient::send()
{
	Socket::send(this->_response.read());
}

HttpRequest &HttpClient::request()
{
	return (this->_request);
}

HttpResponse &HttpClient::response()
{
	return (this->_response);
}

const HttpRequest &HttpClient::request() const
{
	return (this->_request);
}

const HttpResponse &HttpClient::response() const
{
	return (this->_response);
}