#ifndef CONFIGPARSER_HPP
# define CONFIGPARSER_HPP

# include <iostream>
# include "ServerConfig.hpp"
# include "File.hpp"

class ConfigParser
{
public:
    // Default Constructor
    ConfigParser(File config);
    // Destructor
    ~ConfigParser();
};

#endif
