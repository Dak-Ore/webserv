#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include <iostream>
# include "LocationConfig.hpp"
# include <sstream>
# include <map>
# include <cstdlib>
# include <vector>


class ServerConfig : public Config
{
public:
    // Default Constructor
    ServerConfig(std::string content, std::vector<std::string> location);
    
    // Destructor
    ~ServerConfig();

	void	findElement(std::string line);
    std::vector<std::string>  getHost();
    std::vector<std::string> getPorts();
	std::vector<std::string> getServerNames();
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
    std::vector<LocationConfig> _locations;
};

#endif
