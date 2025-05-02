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
			std::string locPath = utils::addTrailingSlash(location->getPath());
			std::string normPath = utils::addTrailingSlash(path);

			std::cout << "   - LOCATION: " << locPath << std::endl;
			if (normPath.compare(0, locPath.length(), locPath) == 0)
				relativePath = utils::removeTrailingSlash(path.substr(locPath.length() - 1));
			else
				relativePath = "";
		}
		std::string file_path = utils::joinPath(root, relativePath);
		if (utils::isDirectory(file_path))
			file_path = this->getIndex(root, relativePath);
		std::cout << "   - FILE: " <<  file_path << std::endl;
		response.setBodySource(file_path);
	}
	response.send(response_fd);
	this->_epoll.remove(response_fd);
	return (true);
}

std::string Server::getIndex(const std::string& root, const std::string& path)
{
	const std::vector<std::string>& indexList = this->_config.getIndex();
	std::string directory = utils::addTrailingSlash(utils::joinPath(root, path));

	for (size_t i = 0; i < indexList.size(); ++i)
	{
		std::string file_path = directory + indexList[i];
		if (utils::fileExists(file_path))
			return (file_path);
	}
	return std::string();
}

const LocationConfig* Server::matchLocation(const HttpRequest& request)
{
	std::string path = utils::addTrailingSlash(request.getPath());
	const LocationConfig* bestMatch = NULL;
	size_t bestLength = 0;

	const std::vector<LocationConfig>& locations = this->_config.getLocations();
	for (size_t i = 0; i < locations.size(); i++)
	{
		std::string locPath = utils::addTrailingSlash(locations[i].getPath());
		if (path.compare(0, locPath.length(), locPath) == 0 && locPath.length() > bestLength)
		{
			bestMatch = &locations[i];
			bestLength = locPath.length();
		}
	}
	return (bestMatch);
}