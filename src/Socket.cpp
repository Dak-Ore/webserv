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

Socket::~Socket()
{
	// this->close();
}
void Socket::setFd(int fd)
{
	this->close();
	this->_fd = fd;
}

int Socket::getFd() const {return this->_fd;}
const Adress &Socket::getAdress() const {return this->_adress;}

void Socket::close()
{
	if (this->_fd != -1)
		::close(this->_fd);
	this->_fd = -1;
}

std::string Socket::read()
{
	std::string content;
	size_t size = 0;
	char buffer[1024];
	int bytes;

	while (size < 4096)
	{
		bytes = ::read(this->getFd(), buffer, sizeof(buffer));
		if (bytes <= 0)
			break ;
		size += bytes;
		content.append(buffer, bytes);
	}
	return (content);
}

void Socket::send(std::string &response)
{
	size_t bytes;
	bytes = ::send(this->_fd , response.c_str(), response.size(), MSG_NOSIGNAL);
	response.erase(0, bytes);
}