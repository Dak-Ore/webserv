#include "HttpClient.hpp"
#include "Socket.hpp"

HttpClient::HttpClient()
{
}

HttpClient::HttpClient(int serverFd)
{
	this->_fd = ::accept(serverFd, NULL, NULL);
	if (this->_fd == -1)
		return ;
	std::pair<int, int> info = Socket::getSocketInfo(this->_fd);
	this->_host = info.first;
	this->_port = info.second;
}

HttpClient::HttpClient(HttpClient const &ref)
{
	*this = ref;
}
HttpClient &HttpClient::operator=(HttpClient const &ref)
{
	this->_fd = ref._fd;
	this->_host = ref._host;
	this->_port = ref._port;
	return (*this);
}

HttpClient::~HttpClient()
{
}


int HttpClient::getFd() const {return this->_fd;}
int HttpClient::getHost() const {return this->_host;}
int HttpClient::getPort() const {return this->_port;}
