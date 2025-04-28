#include "HttpClient.hpp"
#include "Socket.hpp"
#include "Adress.hpp"

HttpClient::HttpClient() : Socket()
{
}

HttpClient::HttpClient(int serverFd) : Socket()
{
	this->_fd = ::accept(serverFd, NULL, NULL);
	if (this->_fd == -1)
		return ;
	this->_adress = Adress(this->_fd);
}

HttpClient::HttpClient(HttpClient const &ref) : Socket()
{
	*this = ref;
}
HttpClient &HttpClient::operator=(HttpClient const &ref)
{
	this->_fd = ref._fd;
	this->_adress = ref._adress;
	return (*this);
}

HttpClient::~HttpClient()
{
}
