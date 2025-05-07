#ifndef CONFIGPARSER_HPP
# define CONFIGPARSER_HPP

# include <iostream>
# include "ServerConfig.hpp"
# include "File.hpp"
# include <string>
# include <cctype>

/**
 * Creates ServerConfig-s from a config file.
 */
class ConfigParser
{
public:
    // Constructor
    ConfigParser(File config);
    // Destructor
    ~ConfigParser();
    /**
     * Get the ServerConfig-s.
     */
	std::vector<ServerConfig>	getServer();
private:
	std::vector<ServerConfig>	_server;
	void addServer(std::string content, std::vector<std::string> location);
	bool processServerStart(const std::string& line, int& waitBrace);
	bool processLocationStart(const std::string& line, int& waitBrace);
	bool processOpeningBrace(const std::string& line, int& waitBrace, int& inServer, int& inLocation);
	bool processFullLineServer(const std::string& line, int& inServer);
	bool processFullLineLocation(const std::string& line, int& inLocation);

};

#endif
