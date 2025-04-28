#include "Socket.hpp"
#include "utils.hpp"

#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <string>
#include <stdexcept>
#include <iostream>

Socket::Socket()
{
	this->_fd = -1;
}

Socket::Socket(std::string hostname, std::string service)
{
	Adress adress(hostname, service);
	this->_fd = adress.createSocket();
	if (!adress.bind(this->_fd))
	{
		::close(this->_fd);
		throw std::runtime_error("Failed to bind socket");
	}
	this->listen();
}

Socket::~Socket()
{
}

void Socket::listen()
{
	if (this->_fd < 0)
		throw std::runtime_error("Invalid socket file descriptor");

	if (::listen(this->_fd, SOMAXCONN) != 0)
		throw std::runtime_error("Failed to listen on socket");
}

int Socket::getFd() const {return this->_fd;}
const Adress &Socket::getAdress() const {return this->_adress;}

void Socket::close()
{
	if (this->_fd != -1)
		::close(this->_fd);
	this->_fd = -1;
}