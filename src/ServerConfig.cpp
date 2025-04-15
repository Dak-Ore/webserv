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
	std::string	elements[] = {"listen", "server_name", "root", "index", "client_max_body_size", "error_page"};
	int i = 0;
	size_t pos;
	for (i = 0; i < 6; i++)
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
			this->splitPush(smartSubstr(line, "server_name", ";"), 1);
			break;
		case 2:
			this->_root = smartSubstr(line, "root", ";");
			break;
		case 3:
			this->splitPush(smartSubstr(line, "index", ";"), 0);
			break;
		case 4:
			this->_clientMaxBodySize = atoi(smartSubstr(line, "client_max_body_size", ";").c_str());
			break;
		case 5:{
			std::string pLine = smartSubstr(line, "error_pages", "/");
			this->_errorPages.insert(std::pair<int,std::string>(atoi(pLine.c_str()), smartSubstr(line, pLine, ";")));
			break;
		}
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
std::vector<std::string> ServerConfig::getIndex()
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

void	ServerConfig::print()
{
	std::cout << "My host:" << std::endl;
	for (std::vector<std::string>::iterator it = this->_host.begin(); it != this->_host.end(); it++)
		std::cout << *it << std::endl;
	std::cout << "My port:" << std::endl;
	for (std::vector<std::string>::iterator it = this->_ports.begin(); it != this->_ports.end(); it++)
		std::cout << *it << std::endl;
	std::cout << "Root: " << this->_root << std::endl;
	std::cout << "Index" << std::endl;
	for (std::vector<std::string>::iterator it = this->_index.begin(); it != this->_index.end(); it++)
		std::cout << *it << std::endl;
	std::cout <<  "Client max body size: " << _clientMaxBodySize << std::endl;
	std::cout << "Error pages" << std::endl;
	for (std::map<int, std::string>::iterator it = this->_errorPages.begin(); it != this->_errorPages.end(); it++)
		std::cout << it->first << " : " << it->second << std::endl;
	
}

void	ServerConfig::splitPush(std::string line, int kind)
{
	std::istringstream	stream(line);
	std::string			word;

	while (stream >> word)
	{
		if (kind == 0)
			this->_index.push_back(word);
		else
			this->_serverNames.push_back(word);
	}


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