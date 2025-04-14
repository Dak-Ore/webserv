#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include <iostream>

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
	std::vector<int> ports;
	std::string
};

#endif
