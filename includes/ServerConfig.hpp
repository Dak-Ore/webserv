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
    virtual ~ServerConfig();
	void	findElement(std::string line);
    std::vector<std::pair<std::string, std::string> > getAdress();
	std::vector<std::string> getServerNames();
    std::vector<LocationConfig> getLocations();
	void print();
	void checkConfig();
private:
    std::vector<std::pair<std::string, std::string> > _adress;
	std::vector<std::string> _serverNames;
    std::vector<LocationConfig> _locations;
};

#endif
