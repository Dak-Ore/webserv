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
	bool processServerStart(const std::string& line, int& waitBrace, int inServer);
	bool processLocationStart(const std::string& line, int& waitBrace, int inLocation);
	bool processOpeningBrace(const std::string& line, int& waitBrace, int& inServer, int& inLocation);
	bool processFullLineServer(const std::string& line, int& inServer);
	bool processFullLineLocation(const std::string& line, int& inLocation);

};

#endif
