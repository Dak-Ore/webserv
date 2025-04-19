#include "Server.hpp"
#include "EPoll.hpp"
#include "ServerConfig.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "utils.hpp"

#include <stdexcept>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sys/socket.h>

Server::Server(EPoll &epoll_ref, ServerConfig &config) :
	_epoll(epoll_ref), _config(config)
{
	Socket *socket;
	for (size_t i = 0; i < config.getHost().size(); i++)
	{
		socket = new Socket(config.getHost()[i], config.getPorts()[i]);
		std::cout << "Server launched on " << config.getHost()[i] << ":" << config.getPorts()[i] << std::endl;
		this->_sockets.push_back(socket);
		this->_epoll.addSocket(socket->getFd());
	}
}

Server::~Server()
{
	for (size_t i = 0; i < this->_sockets.size(); ++i)
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

int Server::acceptClient(int serverFd)
{
	int client_fd = ::accept(serverFd, NULL, NULL);
	if (client_fd != -1)
		this->_epoll.addClient(client_fd);
	return (client_fd);
}

HttpRequest Server::readRequest(int fd)
{
	std::string request_string;
	char buffer[1024];

	while (true)
	{
		int bytes = ::recv(fd, buffer, sizeof(buffer), 0);
		if (bytes <= 0) break;
		request_string.append(buffer, bytes);

		size_t pos = request_string.find("\r\n\r\n");
		if (pos != std::string::npos) {
			request_string = request_string.substr(0, pos + 4);
			// Body
			while (::recv(fd, buffer, sizeof(buffer), MSG_DONTWAIT) > 0)
				continue ;
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
		std::string base("." + this->_config.getRoot() + "/");
		size_t	i = 0;
		std::vector<std::string> index = this->_config.getIndex();
		while (i < index.size() && !utils::fileExists(base + index[i]))
			i++;
		if (i == index.size())
			return (false); // a gerer plus tard
		std::string file_path = utils::joinPath(base, (request.getPath() == "/") ? index[i] : request.getPath());	
		response.setBodySource(file_path);
	}
	std::cout << request.getMethod() << " - " << request.getPath() << std::endl;
	response.send(response_fd);
	this->_epoll.remove(response_fd);
	return (true);
}