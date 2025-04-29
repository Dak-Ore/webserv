#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include "Adress.hpp"
# include "LocationConfig.hpp"
# include <sstream>
# include <map>
# include <vector>


class ServerConfig : public Config
{
public:
    // Default Constructor
    ServerConfig(std::string content, std::vector<std::string> location);
    // Destructor
    virtual ~ServerConfig();
	void	findElement(std::string line);
    std::vector<Adress> getAdress();
	std::vector<std::string> getServerNames();
    std::vector<LocationConfig> getLocations();
	void print();
	void checkConfig();
private:
    std::vector<Adress>_adress;
	std::vector<std::string> _serverNames;
    std::vector<LocationConfig> _locations;
};

#endif
