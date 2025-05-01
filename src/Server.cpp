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
	const std::string path = request.getPath();
	std::cout << request.getMethod() << " - " << path << std::endl;
	const LocationConfig* location = this->matchLocation(request);
	const std::string& root = location ? location->getRoot() : this->_config.getRoot();

	int code;
	if (!request.isValid(&code))
		response = HttpResponse(code);
	else
	{
		std::string relativePath = path;
		if (location)
		{
			const std::string& locPath = location->getPath();
			if (path.compare(0, locPath.size(), locPath) == 0)
				relativePath = path.substr(locPath.size());
		}
		std::string file_path = utils::joinPath(root, relativePath);
		if (utils::isDirectory(file_path))
			file_path = this->getIndex(root, relativePath);
		std::cout << "FILE: " <<  file_path << std::endl;
		response.setBodySource(file_path);
	}
	response.send(response_fd);
	this->_epoll.remove(response_fd);
	return (true);
}

std::string Server::getIndex(const std::string& root, const std::string& path)
{
	std::vector<std::string> indexList = this->_config.getIndex();
	std::string directory = utils::joinPath(root, path);
	if (!directory.empty() && directory.at(directory.size() - 1) != '/')
		directory += '/';

	for (size_t i = 0; i < indexList.size(); ++i)
	{
		std::string file_path = directory + indexList[i];
		if (utils::fileExists(file_path))
			return (file_path);
	}
	return std::string();
}

const LocationConfig *Server::matchLocation(const HttpRequest& request)
{
	const std::string& path = request.getPath();
	const LocationConfig* bestMatch = NULL;
	size_t bestLength = 0;

	const std::vector<LocationConfig>& locations = this->_config.getLocations();
	for (size_t i = 0; i < locations.size(); i++)
	{
		const std::string& locPath = locations[i].getPath();
		if (path.compare(0, locPath.length(), locPath) == 0 && locPath.length() > bestLength)
		{
			bestMatch = &locations[i];
			bestLength = locPath.length();
		}
	}
	return (bestMatch);
}