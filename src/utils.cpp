#include "utils.hpp"

#include <sstream>
#include <sys/stat.h>
#include <stdint.h>

std::string utils::numToString(size_t value)
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

bool utils::isDirectory(const std::string &path)
{
	struct stat fileInfo;
	return (stat(path.c_str(), &fileInfo) == 0 && S_ISDIR(fileInfo.st_mode));
}


off_t utils::getFileSize(const std::string& path) {
    struct stat st;
    if (stat(path.c_str(), &st) == -1)
        return -1;
    return (st.st_size);
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

std::string	utils::extractKey(std::string line)
{
	std::istringstream iss(utils::trim(line));
	std::string word;
	iss >> word;
	return word;
}

std::string	utils::extractElem(std::string line, int pos)
{
	std::istringstream iss(utils::trim(line));
	std::string word;
	for (int i = 0; i < pos; i++)
		iss >> word;
	return word;
}
void utils::ft_split(std::string line, std::vector<std::string> *list)
{
	std::istringstream	stream(line);
	std::string			word;
	while (stream >> word)
		list->push_back(word);
}


bool utils::isValidRegex(std::string str, std::string pattern) 
{
    regex_t regex;

    if (regcomp(&regex, pattern.c_str(), REG_EXTENDED | REG_NOSUB) != 0)
        return false;
    int result = regexec(&regex, str.c_str(), 0, NULL, 0);
    regfree(&regex);
    return (result == 0);
}

std::string utils::addTrailingSlash(const std::string& path)
{
	if (!path.empty() && path[path.length() - 1] != '/')
		return path + "/";
	return path;
}

std::string utils::removeTrailingSlash(const std::string& path)
{
	if (!path.empty() && path[path.length() - 1] == '/')
		return path.substr(0, path.length() - 1);
	return path;
}