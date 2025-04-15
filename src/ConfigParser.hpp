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
};

std::string trim(const std::string& s);

#endif
