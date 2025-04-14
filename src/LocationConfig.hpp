#ifndef LOCATIONCONFIG_HPP
# define LOCATIONCONFIG_HPP

# include <iostream>
# include <vector>

class LocationConfig
{
public:
    // Default Constructor
    LocationConfig();
    
    // Copy Constructor
    LocationConfig(const LocationConfig& other);
    
    // Copy Assignment Operator
    LocationConfig& operator=(const LocationConfig& other);
    
    // Destructor
    ~LocationConfig();
private:
	std::string _path;
    std::vector<std::string> _allowedMethods;
    std::string _root;
    std::string _index;
    bool _autoIndex;
    bool _hasRedirection;
    std::pair<int, std::string> _redirection;
    std::string _cgiExtension;
    bool _uploadEnabled;
    std::string _uploadPath;
};

#endif
