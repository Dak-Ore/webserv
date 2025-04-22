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
	const std::string &root = this->_config.getRoot();
	const std::string &path = request.getPath();
	int code;
	if (!request.isValid(&code))
		response = HttpResponse(code);
	else
	{
		std::string file_path = (path == "/") ? 
			this->getIndex(root, path)
			: utils::joinPath(root, path);
		response.setBodySource(file_path);
	}
	std::cout << request.getMethod() << " - " << path << std::endl;
	response.send(response_fd);
	this->_epoll.remove(response_fd);
	return (true);
}

std::string Server::getIndex(std::string root, std::string path)
{
	std::vector<std::string> index = this->_config.getIndex();
	std::string file_path;
	root = utils::joinPath(root, path);
	for (size_t i = 0; i < index.size(); i++)
	{
		file_path = root + index[i];
		if (utils::fileExists(file_path))
			return (file_path);
	}
	return (std::string());
}