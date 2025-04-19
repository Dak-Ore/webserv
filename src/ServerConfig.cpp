#include "ServerConfig.hpp"
#include "utils.hpp"

// Default Constructor
ServerConfig::ServerConfig(std::string content, std::vector<std::string> location)
{
	std::istringstream stream(content);
	std::string		line;

	while (std::getline(stream, line))
		findElement(line);
	for (std::vector<std::string>::iterator it = location.begin(); it != location.end(); it++)
	{
		LocationConfig loc(*it);
		this->_locations.push_back(loc);
	}
}

void	ServerConfig::findElement(std::string line)
{
	std::string key = utils::extractKey(line);
	std::string value = utils::smartSubstr(line, key, ";");

	if (parseVar(key, value, line))
		return ;
	else if (key == "listen")
	{
		this->_host.push_back(utils::smartSubstr(line, key, ":"));
		this->_ports.push_back(utils::smartSubstr(value, ":", ";"));
	}
	else if (key == "server_name")
		utils::ft_split(value, &this->_serverNames);
}

std::vector<std::string>  ServerConfig::getHost(){return this->_host;}
std::vector<std::string> ServerConfig::getPorts(){return this->_ports;}
std::vector<std::string> ServerConfig::getServerNames(){return this->_serverNames;}
std::vector<LocationConfig> ServerConfig::getLocations(){return this->_locations;}
// Destructor
ServerConfig::~ServerConfig() {}

void	ServerConfig::print()
{
	std::cout << "Server" << std::endl << "My host:" << std::endl;
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
	for (size_t i = 0; i < this->_locations.size(); i ++)
		this->_locations[i].print();
	std::cout << std::endl;
	
}
