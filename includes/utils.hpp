#include <string>

namespace utils
{
	std::string intToString(int value);
	bool 		fileExists(const std::string &path);
	std::string joinPath(const std::string &base, const std::string &relative);
	std::string	smartSubstr(std::string line, std::string start, std::string end);
	std::string trim(const std::string& s);

}