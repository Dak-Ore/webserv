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
    ~LocationConfig();

	void		print();
	std::string getPath();
    std::vector<std::string> getAllowedMethods();
    bool getAutoIndex();
    bool getHasRedirection();
    std::pair<int, std::string> getRedirection();
    std::vector<std::string> getCgiExtension();
    bool getUploadEnabled();
    std::vector<std::string> getUploadPath();
private:
	std::string _path;
    std::vector<std::string> _allowedMethods;
    bool _autoIndex;
    bool _hasRedirection;
    std::pair<int, std::string> _redirection;
    std::vector<std::string> _cgiExtension;
    bool _uploadEnabled;
    std::vector<std::string> _uploadPath;
};

#endif
