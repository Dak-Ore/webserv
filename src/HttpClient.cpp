#include "HttpClient.hpp"
#include "Socket.hpp"
#include "Adress.hpp"
#include "utils.hpp"
#include <netdb.h>

HttpClient::HttpClient() : Socket()
{
}

HttpClient::HttpClient(int serverFd) : Socket()
{
	sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    this->_fd = ::accept(serverFd, (struct sockaddr *)&client_addr, &client_len);
	if (this->_fd == -1)
		return ;
	this->_adress = Adress(client_addr);
	this->_serverAdress = Adress(this);
}

HttpClient::HttpClient(HttpClient const &ref) : Socket()
{
	*this = ref;
}
HttpClient &HttpClient::operator=(HttpClient const &ref)
{
	this->_fd = ref._fd;
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