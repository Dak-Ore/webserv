#include "HttpClient.hpp"
#include "Socket.hpp"
#include "Adress.hpp"
#include "utils.hpp"
#include <netdb.h>
#include <string>

HttpClient::HttpClient() : Socket()
{
}

HttpClient::HttpClient(int serverFd) : Socket()
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
	this->request = ref.request;
	this->response = ref.response;
	this->_adress = ref._adress;
	this->_serverAdress = ref._serverAdress;
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
	this->request.read(content);
	return (this->request.isReady());
}

void HttpClient::send()
{
	std::string resp = this->response.toString();
	Socket::send(resp);
}
