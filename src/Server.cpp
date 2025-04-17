#include "Server.hpp"
#include "EPoll.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

#include <stdexcept>
#include <iostream>
#include <fstream>

Server::Server()
{
	this->init("localhost", "80");
}

Server::Server(std::string hostname, std::string service)
{
	this->init(hostname, service);
}

void Server::init(std::string hostname, std::string service)
{
	this->_sockets.push_back(new Socket(hostname, service));
	this->_epoll.addSocket(this->_sockets[0]->getFd());
}

Server::~Server()
{
	for (size_t i = 0; i < _sockets.size(); ++i)
		delete this->_sockets[i];
}

bool Server::isServerSocket(int fd)
{
	size_t n = this->_sockets.size();
	for (size_t i = 0; i < n; i++)
	{
		if (fd == this->_sockets[i]->getFd())
			return (true);
	}
	return (false);
}

void Server::acceptClient(int serverFd)
{
	int client_fd = accept(serverFd, NULL, NULL);
	if (client_fd == -1)
		return ;
	this->_epoll.addClient(client_fd);
}

void Server::listen()
{
	while (true)
	{
		EPollEvent* events = this->_epoll.getEvents();
		for (int i = 0; i < EPOLL_MAX_EVENTS; ++i)
		{
			int fd = events[i].getFd();
			if (fd == 0)
				continue;

			if (this->isServerSocket(fd))
				this->acceptClient(fd);
			else
			{
				std::string request_string = this->readRequest(fd);
				if (request_string.empty())
					this->_epoll.remove(fd);
				else
				{
					HttpRequest request(request_string);
					HttpResponse response(200);
					response.setBodySource("a.html");
					response.send(fd);
					this->_epoll.remove(fd);
				}
			}
		}
	}
}

std::string Server::readRequest(int fd)
{
	std::string request;
	char buffer[1024];

	while (true)
	{
		int bytes = recv(fd, buffer, sizeof(buffer), 0);
		if (bytes <= 0) break;
		request.append(buffer, bytes);

		if (request.find("\r\n\r\n") != std::string::npos)
		{
			// Fin des headers atteinte
			break;
		}
	}
	return (request);
}
