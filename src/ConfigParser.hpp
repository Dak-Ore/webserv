#ifndef CONFIGPARSER_HPP
# define CONFIGPARSER_HPP

# include <iostream>

class ConfigParser
{
public:
    // Default Constructor
    ConfigParser();
    
    // Copy Constructor
    ConfigParser(const ConfigParser& other);
    
    // Copy Assignment Operator
    ConfigParser& operator=(const ConfigParser& other);
    
    // Destructor
    ~ConfigParser();
};

#endif
