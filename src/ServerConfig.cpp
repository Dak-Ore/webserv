#include "ServerConfig.hpp"

// Default Constructor
ServerConfig::ServerConfig(std::string content, std::vector<std::string> location)
{
	(void) location;
	std::istringstream stream(content);
	std::string		line;
	

	while (std::getline(stream, line))
		findElement(line);
}

void	ServerConfig::findElement(std::string line)
{
	std::string	elements[] = {"listen", "server_name", "root", "index", "client_max_body_size", "error_page", "index"};
	int i = 0;
	size_t pos;
	for (i = 0; i < 7; i++)
	{
		pos = line.find(elements[i]);
		if (pos != std::string::npos)
			{
				if (pos != 0)
					throw std::runtime_error("invalid line in config file");
				break;
			}
		}

	switch (i)
	{
		case 0:
			this->_host.push_back(smartSubstr(line, "location", ":"));
			this->_ports.push_back(smartSubstr(line, ":", ";"));
			break;
		case 1:
			this->_serverNames.push_back(smartSubstr(line, "server_name", ";"));
			break;
		case 2:
			this->_root = smartSubstr(line, "root", ";");
			break;
		case 3:
			this->_index = smartSubstr(line, "index", ";");
			break;
		case 4:
			this->_clientMaxBodySize = atoi(smartSubstr(line, "client_max_body_size", ";").c_str());
			break;
		case 5:{
			std::string pLine = smartSubstr(line, "error_pages", "/");
			this->_errorPages.insert({atoi(pLine.c_str()), smartSubstr(line, pLine, ";")});
			break;
		}
		case 6:
			break;
		case 7:
			break;
		default:
			throw std::runtime_error("invalid line in config file");
	}
}

std::vector<std::string>  ServerConfig::getHost()
{
	return this->_host;
}

std::vector<std::string> ServerConfig::getPorts()
{
	return this->_ports;
}

std::vector<std::string> ServerConfig::getServerNames()
{
	return this->_serverNames;
}
std::string ServerConfig::getRoot()
{
	return this->_root;
}
std::string ServerConfig::getIndex()
{
	return this->_index;
}

size_t ServerConfig::getClientMaxBodySize()
{
	return this->_clientMaxBodySize;
}


std::map<int, std::string> ServerConfig::getErrorPages()
{
	return this->_errorPages;
}
std::vector<LocationConfig> ServerConfig::getLocations()
{
	return this->_locations;
}

// Destructor
ServerConfig::~ServerConfig() {
}

std::string	smartSubstr(std::string line, std::string start, std::string end)
{
	size_t	pos_start = line.find(start) + start.size();
	if (pos_start == std::string::npos)
		pos_start = 0;
	else
	{
		while (isspace(line[pos_start]))
			pos_start++;
	}
	size_t	pos_end = line.find(end);
	if (pos_end == std::string::npos)
		pos_end = 0;
	else
	{
		while (isspace(line[pos_end]))
			pos_end--;
	}
	line = line.substr(pos_start, pos_end - pos_start);
	return (line);
}