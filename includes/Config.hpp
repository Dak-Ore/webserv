#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <iostream>
# include <utils.hpp>
# include <vector>
# include <map>
# include <sstream>
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
    bool _autoIndex;
public:
	std::string getRoot() const;
    std::vector<std::string> getIndex() const;
	size_t getClientMaxBodySize();
    std::map<int, std::string> getErrorPages();
	bool getAutoIndex();
    std::vector<std::string> getCgiExtension();
    bool getUploadEnabled();
    std::vector<std::string> getUploadPath();

	int	parseVar(std::string key, std::string value, std::string line);
	virtual void	findElement(std::string line) = 0;
};

#endif
