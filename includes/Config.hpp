#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <iostream>
# include <utils.hpp>
# include <vector>
# include <map>
# include <sstream>
# include <cmath>
# include <cstdlib>

class Config
{
protected:
    std::string _root;
    std::vector<std::string> _index;
    size_t _clientMaxBodySize;
    std::map<int, std::string> _errorPages;
    std::vector<std::string> _cgiExtension;
    bool _uploadEnabled;
    std::vector<std::string> _uploadPath;
    std::vector<std::string> _allowedMethods;
    bool _autoIndex;
public:
	Config();
	~Config();
	std::string getRoot() const;
    std::vector<std::string> getIndex() const;
	size_t getClientMaxBodySize() const;
    std::map<int, std::string> getErrorPages() const;
	bool getAutoIndex() const;
    std::vector<std::string> getCgiExtension()const;
    bool getUploadEnabled() const;
    std::vector<std::string> getUploadPath()const;
	std::vector<std::string> getAllowedMethods()const;
	void checkVar();
	int	parseVar(std::string key, std::string value, std::string line);
	virtual void	findElement(std::string line) = 0;
};

#endif
