#include "Socket.hpp"
#include "utils.hpp"

#include <sys/socket.h>
#include <netdb.h>
#include <string>
#include <stdexcept>
#include <iostream>

std::pair<int, int> Socket::getSocketInfo(int fd)
{
	std::pair<int, int> adress;;

	struct sockaddr_in server_addr;
	socklen_t addrlen = sizeof(server_addr);
	if (getsockname(fd, (struct sockaddr*)&server_addr, &addrlen) != -1)
	{
		adress.first = server_addr.sin_addr.s_addr;
		adress.second = ntohs(server_addr.sin_port);
	}
	return (adress);
}

Socket::Socket(std::string hostname, std::string service)
{
	this->_fd = -1;
	struct addrinfo hints;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
	struct addrinfo *res;
	int status = ::getaddrinfo(hostname.c_str(), service.c_str(), &hints, &res);
	if (status != 0)
		throw std::runtime_error(::gai_strerror(status));
	this->_fd = ::socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (this->_fd == -1)
	{
		::freeaddrinfo(res);
		throw std::runtime_error("Failed to create socket");
	}
	int i = 1;
	if (::setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(i)) < 0)
	{
		::close(this->_fd);
		::freeaddrinfo(res);
		throw std::runtime_error("setsockopt SO_REUSEADDR failed");
	}
	if (::bind(this->_fd, res->ai_addr, res->ai_addrlen) != 0)
	{
		::close(this->_fd);
		::freeaddrinfo(res);
		throw std::runtime_error("Failed to bind socket");
	}
	::freeaddrinfo(res);
	this->setSocketInfo();
	this->listen();
}

Socket::~Socket()
{
	if (this->_fd >= 0)
		::close(this->_fd);
}

void Socket::setSocketInfo()
{
	std::pair<int, int> info = Socket::getSocketInfo(this->_fd);
	this->_host = info.first;
	this->_port = info.second;
}

int Socket::getFd()
{
	return (this->_fd);
}

void Socket::listen()
{
	if (this->_fd < 0)
		throw std::runtime_error("Invalid socket file descriptor");

	if (::listen(this->_fd, SOMAXCONN) != 0)
		throw std::runtime_error("Failed to listen on socket");
}