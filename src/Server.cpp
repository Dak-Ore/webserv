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
