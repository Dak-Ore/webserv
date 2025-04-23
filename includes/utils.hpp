#include <string>
#include <vector>

#include <sys/types.h> 
#include <stdint.h>

namespace utils
{
	std::string numToString(size_t value);
	std::string ip_to_str(int ip);
	bool 		fileExists(const std::string &path);
	off_t 		getFileSize(const std::string& path);
	std::string joinPath(const std::string &base, const std::string &relative);
	std::string	smartSubstr(std::string line, std::string start, std::string end);
	std::string trim(const std::string& s);
	std::string	extractKey(std::string line);
	void ft_split(std::string line, std::vector<std::string> *list);


}