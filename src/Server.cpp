#include "Server.hpp"
#include "EPoll.hpp"
#include "ServerConfig.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "HttpClient.hpp"
#include "utils.hpp"

#include <stdexcept>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sys/socket.h>

Server::Server(ServerConfig &config) :
	_config(config)
{
}

Server::~Server()
{
}

bool Server::handleRequest(HttpRequest const &request, const HttpClient &client)
{
	HttpResponse response;
	response.bindClient(client);
	if (request.empty())
		return (false);
	const std::string path = request.getPath();
	std::cout << request.getMethod() << " - " << request.getHeader("Host")  << path  << std::endl;
	Config* config = this->getConfig(request);
	LocationConfig *location = dynamic_cast<LocationConfig *>(config);
	if (location && location->getHasRedirection())
	{
		response.setCode(location->getRedirection().first);
		response.setHeader("Location", location->getRedirection().second);
		response.send();
		return (true);
	}
	const std::string& root = (config) ? config->getRoot() : this->_config.getRoot();

	if (!request.isValid())
		response.setCode(request.getErrorCode());
	else
	{
		std::string relativePath = (location) ? location->getRelativePath(path) : path;
		std::string file_path = utils::joinPath(root, relativePath);
		if (utils::isDirectory(file_path))
		{
			if (this->_config.getAutoIndex())
			{
				std::string html = utils::generateAutoIndex(file_path, request.getPath());
				response.setBody(html); 
				response.send();
				return (true);
			}
			else
				file_path = this->findIndex(relativePath, location);
		}
		std::cout << "   - FILE: " <<  file_path << std::endl;
		response.setBodySource(file_path);
	}
	std::map<int, std::string>::const_iterator it = this->_config.getErrorPages().find(response.getCode());
	if (it != this->_config.getErrorPages().end())
		response.setBodySource(it->second);
	else if (response.getCode() >= 400 && response.getCode() <= 599)
		response.setBody(utils::generateDefaultError(response.getCode()));
	response.send();
	return (true);
}

std::string Server::findIndex(const std::string& path, const LocationConfig* location)
{
	const std::string& root = (location) ? location->getRoot() : this->_config.getRoot() ;
	const std::vector<std::string>& indexList = (location) ? location->getIndex() : this->_config.getIndex();
	std::string directory = utils::addTrailingSlash(utils::joinPath(root, path));

	for (size_t i = 0; i < indexList.size(); ++i)
	{
		std::string file_path = directory + indexList[i];
		if (utils::fileExists(file_path))
			return (file_path);
	}
	return std::string();
}

Config* Server::getConfig(const HttpRequest& request)
{
	std::string path = utils::addTrailingSlash(request.getPath());
	const Config* bestMatch = NULL;
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
	if (bestMatch == NULL)
		bestMatch = (ServerConfig *)&this->_config;
	return ((Config *)bestMatch);
}