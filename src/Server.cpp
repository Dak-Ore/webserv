#include "Server.hpp"
#include "EPoll.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

#include <stdexcept>
#include <iostream>
#include <fstream>
#include <iomanip>

std::string joinPath(const std::string &base, const std::string &relative) {
	if (base.empty()) return relative;
	if (relative.empty()) return base;

	if (base[base.size() - 1] == '/' && relative[0] == '/')
		return base + relative.substr(1);

	if (base[base.size() - 1] != '/' && relative[0] != '/')
		return base + "/" + relative;

	return (base + relative);
}

Server::Server(std::string hostname, std::string service)
{
	this->_run = true;
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

void Server::stop()
{
	this->_run = false;
}

void Server::listen()
{
	while (this->_run)
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
				HttpRequest const request = this->readRequest(fd);
				if (!this->handleRequest(request, fd))
					this->_epoll.remove(fd);
			}
		}
	}
}

HttpRequest Server::readRequest(int fd)
{
	std::string request_string;
	char buffer[1024];

	while (true)
	{
		int bytes = recv(fd, buffer, sizeof(buffer), 0);
		if (bytes <= 0) break;
		request_string.append(buffer, bytes);

		if (request_string.find("\r\n\r\n") != std::string::npos)
		{
			// Fin des headers atteinte
			break;
		}
	}
	return (HttpRequest(request_string));
}

bool Server::handleRequest(HttpRequest const &request, int response_fd)
{
	HttpResponse response;
	if (request.empty())
		return (false);
	int code;
	if (!request.isValid(&code))
		response = HttpResponse(code);
	else
	{
		std::string base("./www/");
		std::string file_path = joinPath(base, (request.getPath() == "/") ? "/index.html" : request.getPath());	
		response.setBodySource(file_path);
	}
	std::cout << request.getMethod() << " - " << request.getPath() << std::endl;
	response.send(response_fd);
	this->_epoll.remove(response_fd);
	return (true);
}