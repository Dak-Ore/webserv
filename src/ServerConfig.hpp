#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include <iostream>
# include "LocationConfig.hpp"
# include <map>

class ServerConfig
{
public:
    // Default Constructor
    ServerConfig();
    
    // Copy Constructor
    ServerConfig(const ServerConfig& other);
    
    // Copy Assignment Operator
    ServerConfig& operator=(const ServerConfig& other);
    
    // Destructor
    ~ServerConfig();
private:
    std::string _host;
    std::vector<int> _ports;
    std::vector<std::string> _serverNames;
    std::string _root;
    std::string _index;
    size_t _clientMaxBodySize;
    std::map<int, std::string> _errorPages;
    std::vector<LocationConfig> _locations;
};

#endif
