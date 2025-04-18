#ifndef CONFIGPARSER_HPP
# define CONFIGPARSER_HPP

# include <iostream>
# include "ServerConfig.hpp"
# include "File.hpp"
# include <string>
# include <cctype>

class ConfigParser
{
public:
    // Default Constructor
    ConfigParser(File config);
    // Destructor
    ~ConfigParser();
	std::vector<ServerConfig>	getServer();
	size_t	getNbServ();
	void	AddServer(std::string content, std::vector<std::string> location);
private:
	std::vector<ServerConfig>	_server;
	size_t			_nbserv;
};

#endif
