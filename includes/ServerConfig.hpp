#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include "Adress.hpp"
# include "LocationConfig.hpp"
# include <sstream>
# include <map>
# include <vector>


class ServerConfig : public Config
{
private:
    /*
        List of the adress the server need to listen
        Need to specify at least a port to have a valid conf file
        If host not specified default is 0.0.0.0 
    */
    std::vector<Adress>_adress;
    /*
        List of the serve rdomain name
    */
	std::vector<std::string> _serverNames;
    // list of locations
    std::vector<LocationConfig> _locations;
public:
    /**
    	* @brief Parse the content of server and his locations.
        * @param content string that contain all parameters of config file.
        * @param location list of contents of the location(s) of the server.
    */
    ServerConfig(std::string content, std::vector<std::string> location);
    // Delete
    virtual ~ServerConfig();
    /**
        * @brief extract key and value of the line and search if it's a valid parameter.
        * @param line contain a line of content 
     */
	void	findElement(std::string line);

    // GETTERS
    const std::vector<Adress>& getAdress() const;
	const std::vector<std::string>& getServerNames() const;
    const std::vector<LocationConfig>& getLocations() const;

    /*
        Check is parsed var is valid.
        current check : 
            - address : is not empty and has valid character
            - server name : has valid character
    */
	void checkConfig();

    // DEBUG
    void print();
};

#endif
