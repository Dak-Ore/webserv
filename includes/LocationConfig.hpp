#ifndef LOCATIONCONFIG_HPP
# define LOCATIONCONFIG_HPP

# include <iostream>
# include <vector>
# include <sstream>
# include "File.hpp"
# include "Config.hpp"
# include <cstdlib>

class LocationConfig : public Config
{
private:
	std::string _path;
    bool _hasRedirection;
    std::pair<int, std::string> _redirection;
public:
    // Default Constructor
    LocationConfig(std::string content);
	// Destructor
    virtual ~LocationConfig();
    /**
        * @brief extract key and value of the line and search if it's a valid parameter.
        * @param line contain a line of content 
     */
	void	findElement(std::string line);
    /*
        Check is parsed var is valid.
        current check : 
            - path check if character is valid
            - redirection check if int part is correct (>= 100 OR <= 599) and string part contains valid charecter
    */
	void		checkConfig();

    /**
        * TO DO 
     */
    std::string getRelativePath(const std::string& path) const;
    
    // GETTER
    std::pair<int, std::string> getRedirection() const;
    std::string getPath() const;
    bool getHasRedirection() const;

    // DEBUG
	void		print();
};

#endif
