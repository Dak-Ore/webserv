#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include <iostream>
# include "LocationConfig.hpp"
# include <sstream>
# include <map>
# include <cstdlib>

class ServerConfig
{
public:
    // Default Constructor
    ServerConfig(std::string content, std::vector<std::string> location);
    
    // Copy Constructor
    ServerConfig(const ServerConfig& other);
    
    // Copy Assignment Operator
    ServerConfig& operator=(const ServerConfig& other);
    
    // Destructor
    ~ServerConfig();

	void	findElement(std::string line);
    std::vector<std::string>  getHost();
    std::vector<std::string> getPorts();
	std::vector<std::string> getServerNames();
    std::string getRoot();
    std::vector<std::string> getIndex();
    size_t getClientMaxBodySize();
    std::map<int, std::string> getErrorPages();
    std::vector<LocationConfig> getLocations();
	void	splitPush(std::string line, int kind);

	void print();
private:
	// listen
    std::vector<std::string> _host;
    std::vector<std::string> _ports;
	//
	std::vector<std::string> _serverNames;
    std::string _root;
    std::vector<std::string> _index;
    size_t _clientMaxBodySize;
    std::map<int, std::string> _errorPages;
    std::vector<LocationConfig> _locations;
};

std::string	smartSubstr(std::string line, std::string start, std::string end);

#endif
