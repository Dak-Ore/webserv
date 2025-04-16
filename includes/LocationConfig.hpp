#ifndef LOCATIONCONFIG_HPP
# define LOCATIONCONFIG_HPP

# include <iostream>
# include <vector>
# include <sstream>
# include "File.hpp"
# include <cstdlib>

class LocationConfig
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
    std::string getRoot();
    std::vector<std::string> getIndex();
    bool getAutoIndex();
    bool getHasRedirection();
    std::pair<int, std::string> getRedirection();
    std::vector<std::string> getCgiExtension();
    bool getUploadEnabled();
    std::vector<std::string> getUploadPath();
private:
	std::string _path;
    std::vector<std::string> _allowedMethods;
    std::string _root;
    std::vector<std::string> _index;
    bool _autoIndex;
    bool _hasRedirection;
    std::pair<int, std::string> _redirection;
    std::vector<std::string> _cgiExtension;
    bool _uploadEnabled;
    std::vector<std::string> _uploadPath;
};

#endif
