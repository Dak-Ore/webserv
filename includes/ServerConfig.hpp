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
    ServerConfig(std::string content, std::vector<std::string> location);
    virtual ~ServerConfig();
	void	findElement(std::string line);
    const std::vector<Adress>& getAdress();
	const std::vector<std::string>& getServerNames();
    const std::vector<LocationConfig>& getLocations();
	void print();
	void checkConfig();
private:
    std::vector<Adress>_adress;
	std::vector<std::string> _serverNames;
    std::vector<LocationConfig> _locations;
};

#endif
