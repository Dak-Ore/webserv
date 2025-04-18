#include "utils.hpp"

#include <sstream>
#include <sys/stat.h>

std::string utils::intToString(int value)
{
	std::ostringstream oss;
	oss << value;
	return oss.str();
}

bool utils::fileExists(const std::string &path)
{
	struct stat fileInfo;
	return (stat(path.c_str(), &fileInfo) == 0 && S_ISREG(fileInfo.st_mode));
}

std::string utils::joinPath(const std::string &base, const std::string &relative)
{
	if (base.empty()) return relative;
	if (relative.empty()) return base;

	if (base[base.size() - 1] == '/' && relative[0] == '/')
		return base + relative.substr(1);

	if (base[base.size() - 1] != '/' && relative[0] != '/')
		return base + "/" + relative;

	return (base + relative);
}

std::string	utils::smartSubstr(std::string line, std::string start, std::string end)
{
	size_t	pos_start = line.find(start) + start.size();
	if (pos_start == std::string::npos)
		pos_start = 0;
	else
	{
		while (isspace(line[pos_start]))
			pos_start++;
	}
	size_t	pos_end = line.find(end);
	if (pos_end == std::string::npos)
		pos_end = 0;
	else
	{
		while (isspace(line[pos_end]))
			pos_end--;
	}
	line = line.substr(pos_start, pos_end - pos_start);
	return (line);
}

std::string utils::trim(const std::string& s)
{
	size_t start = 0;
	while (start < s.length() && std::isspace(s[start]))
		start++;

	size_t end = s.length();
	while (end > start && std::isspace(s[end - 1]))
		end--;

	return s.substr(start, end - start);
}