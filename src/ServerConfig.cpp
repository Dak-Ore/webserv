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
	this->checkConfig();
}

void	ServerConfig::findElement(std::string line)
{
	std::string key = utils::extractKey(line);
	std::string value = utils::smartSubstr(line, key, ";");

	if (parseVar(key, value, line))
		return ;
	else if (key == "listen")
	{
		if (line.find(":") == std::string::npos)
			this->_adress.push_back(std::pair<std::string, std::string>("0.0.0.0", utils::smartSubstr(line, key, ";")));
		else
			this->_adress.push_back(std::pair<std::string, std::string>(utils::smartSubstr(line, key, ":"), utils::smartSubstr(value, ":", ";")));
	}
	else if (key == "server_name")
		utils::ft_split(value, &this->_serverNames);
}

void ServerConfig::checkConfig()
{
	// ADRESS PART
	for (std::vector<std::pair<std::string, std::string> >::iterator it = this->_adress.begin(); it != this->_adress.end(); it++)
	{
		if (!utils::isValidRegex(it->second, "^[0-9]+$") || !utils::isValidRegex(it->first, "^[0-9]{1,3}(\\.[0-9]{1,3}){3}$"))
			throw std::runtime_error("Invalid adress in config file");
		int	port = atoi(it->second.c_str());
		if (port < 1 || port > 65535)
			throw std::runtime_error("Invalid port in config file");
	}
	// SERVER NAME
	for (std::vector<std::string>::iterator it = this->_serverNames.begin(); it != this->_serverNames.end(); it++)
		if (!utils::isValidRegex(*it, "^[A-Za-z0-9_.-]+$"))
			throw std::runtime_error("Invalid server name in config file");
}

std::vector<std::pair<std::string, std::string> > ServerConfig::getAdress(){return this->_adress;}
std::vector<std::string> ServerConfig::getServerNames(){return this->_serverNames;}
std::vector<LocationConfig> ServerConfig::getLocations(){return this->_locations;}
// Destructor
ServerConfig::~ServerConfig() {}

void	ServerConfig::print()
{
	std::cout << "Server" << std::endl << "My host:" << std::endl;
	for (std::vector<std::pair<std::string, std::string> >::iterator it = this->_adress.begin(); it != this->_adress.end(); it++)
		std::cout << it->first << ":" << it->second << std::endl;
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
