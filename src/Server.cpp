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
}

Server::~Server()
{
}

bool Server::handleRequest(HttpRequest const &request, int response_fd)
{
	HttpResponse response;
	if (request.empty())
		return (false);
	const std::string &root = this->_config.getRoot();
	const std::string &path = request.getPath();
	std::cout << request.getMethod() << " - " << path << std::endl;
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