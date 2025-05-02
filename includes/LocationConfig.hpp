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
public:
    // Default Constructor
    LocationConfig(std::string content);
    
	void	findElement(std::string line);
    void	splitPush(std::string line, int kind);
	// Destructor
    virtual ~LocationConfig();
	void		checkConfig();
	void		print();
	std::string getPath() const;
    bool getHasRedirection() const;
    std::pair<int, std::string> getRedirection() const;
private:
	std::string _path;
    bool _hasRedirection;
    std::pair<int, std::string> _redirection;
};

#endif
