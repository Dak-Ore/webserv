#include "ServerSocket.hpp"
#include "Adress.hpp"
#include <string>

ServerSocket::ServerSocket() : Socket()
{}

ServerSocket::ServerSocket(const Adress &adress) : Socket()
{
	this->setFd(adress.createSocket());
	if (!adress.bind(this->getFd()))
	{
		this->close();
		throw std::runtime_error("Failed to bind socket");
	}
	this->listen();
}


void ServerSocket::listen()
{
	if (this->getFd() < 0)
		throw std::runtime_error("Invalid socket file descriptor");

	if (::listen(this->getFd(), SOMAXCONN) != 0)
		throw std::runtime_error("Failed to listen on socket");
}