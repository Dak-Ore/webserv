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
	std::string	elements[] = {"listen", "server_name", "root", "index", "client_max_body_size", "error_page"};
	int i = 0;
	size_t pos;
	std::string elemLine;
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

	if (i < 6)
		elemLine = utils::smartSubstr(line, elements[i], ";");
	switch (i)
	{
		case 0:
			this->_host.push_back(utils::smartSubstr(line, "location", ":"));
			this->_ports.push_back(utils::smartSubstr(line, ":", ";"));
			break;
		case 1:
			this->splitPush(elemLine, 1);
			break;
		case 2:
			this->_root = elemLine;
			break;
		case 3:
			this->splitPush(elemLine, 0);
			break;
		case 4:
			this->_clientMaxBodySize = atoi(elemLine.c_str());
			break;
		case 5:{
			std::string pLine = utils::smartSubstr(line, "error_pages", "/");
			this->_errorPages.insert(std::pair<int,std::string>(atoi(pLine.c_str()), utils::smartSubstr(line, pLine, ";")));
			break;
		}
		default:
			throw std::runtime_error("invalid line in config file");
	}
}

std::vector<std::string>  ServerConfig::getHost(){return this->_host;}
std::vector<std::string> ServerConfig::getPorts(){return this->_ports;}
std::vector<std::string> ServerConfig::getServerNames(){return this->_serverNames;}
std::string ServerConfig::getRoot(){return this->_root;}
std::vector<std::string> ServerConfig::getIndex(){return this->_index;}
size_t ServerConfig::getClientMaxBodySize(){return this->_clientMaxBodySize;}
std::map<int, std::string> ServerConfig::getErrorPages(){return this->_errorPages;}
std::vector<LocationConfig> ServerConfig::getLocations(){return this->_locations;}
// Destructor
ServerConfig::~ServerConfig() {
}

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