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
	const std::string &path = request.getPath();
	std::cout << request.getMethod() << " - " << request.getHeader("Host")  << path  << std::endl;
	Config* config = request.getConfig();
	LocationConfig *location = dynamic_cast<LocationConfig *>(config);
	if (this->handleRedirect(response, location))
		return (true);

	if (!request.isValid())
		response.setCode(request.getErrorCode());
	else
	{
		std::string relativePath = config->getRelativePath(request.getPath());
		std::string file_path = utils::joinPath(config->getRoot(), relativePath);
		if (utils::isDirectory(file_path))
		{
			if (config->getAutoIndex())
				return (this->sendAutoindex(request, response, file_path), true);
			else
				file_path = this->findIndex(relativePath, location);
		}
		std::cout << "   - FILE: " <<  file_path << std::endl;
		response.setBodySource(file_path);
	}
	this->handleErrorPages(response, config);
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

bool Server::handleRedirect(HttpResponse &response, LocationConfig *location)
{
	if (!location || !location->getHasRedirection())
		return (false);
	response.setCode(location->getRedirection().first);
	response.setHeader("Location", location->getRedirection().second);
	response.send();
	return (true);
}

void Server::sendAutoindex(const HttpRequest &request, HttpResponse &response, const std::string &directory)
{
	std::string html = utils::generateAutoIndex(directory, request.getPath());
	response.setBody(html); 
	response.send();
}

void Server::handleErrorPages(HttpResponse &response, Config *config)
{
	std::map<int, std::string>::const_iterator it = config->getErrorPages().find(response.getCode());
	if (!response.hasBody() && it != config->getErrorPages().end())
		response.setBodySource(it->second);
	if (!response.hasBody() && response.getCode() >= 400 && response.getCode() <= 599)
		response.setBody(utils::generateDefaultError(response.getCode()));
}
