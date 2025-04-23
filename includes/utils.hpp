#include <string>
#include <vector>
#include <regex.h>
#include <sys/types.h> 

namespace utils
{
	std::string numToString(size_t value);
	bool 		fileExists(const std::string &path);
	off_t 		getFileSize(const std::string& path);
	std::string joinPath(const std::string &base, const std::string &relative);
	std::string	smartSubstr(std::string line, std::string start, std::string end);
	std::string trim(const std::string& s);
	std::string	extractKey(std::string line);
	void ft_split(std::string line, std::vector<std::string> *list);
	bool isValidRegex(std::string str, std::string pattern); 
	bool isDirectory(const std::string &path);
}