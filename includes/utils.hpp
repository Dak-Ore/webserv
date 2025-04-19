#include <string>
#include <vector>

namespace utils
{
	std::string intToString(int value);
	bool 		fileExists(const std::string &path);
	std::string joinPath(const std::string &base, const std::string &relative);
	std::string	smartSubstr(std::string line, std::string start, std::string end);
	std::string trim(const std::string& s);
	std::string	extractKey(std::string line);
	void ft_split(std::string line, std::vector<std::string> *list);


}