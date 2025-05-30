#include "utils.hpp"

#include <sstream>
#include <cctype>
#include <cstdlib>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>
#include <algorithm>
#include <stdexcept>
#include <map>
#include <string>
#include <vector>
#include <cstring>
#include <sstream>
#include <cstdio>
#include <dirent.h>

std::string utils::numToString(size_t value)
{
	std::ostringstream oss;
	oss << value;
	return oss.str();
}

size_t utils::stringToNum(std::string str)
{
	return atoi(str.c_str());
}


int utils::stringToInt(const std::string& str)
{
	std::stringstream ss(str);
	int num = 0;
	ss >> num;
	return num;
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

static const char** vector_to_c_array(std::vector<std::string> const& a)
{
	const char** r = new const char*[a.size() + 1];
	{
		size_t i = 0;
		std::vector<std::string>::const_iterator it = a.begin();
		while (it != a.end()) {
			r[i] = it->c_str();
			i++;
			it++;
		}
		r[i] = NULL;
	}
	return r;
}

static const char** map_to_c_array(std::map<std::string, std::string> const& a)
{
	const char** r = new const char*[a.size() + 1];
	{
		size_t i = 0;
		std::map<std::string, std::string>::const_iterator it = a.begin();
		while (it != a.end()) {
			r[i] = strdup((it->first + std::string("=") + it->second).c_str());
			i++;
			it++;
		}
		r[i] = NULL;
	}
	return r;
}

pid_t utils::forkexec(int inout[2], std::vector<std::string> const argv, std::map<std::string, std::string> const envp)
{
	// create pipes
	int pipein[2];
	if (pipe(pipein) < 0)
		throw std::runtime_error("pipe() failed.");
	int pipeout[2];
	if (pipe(pipeout) < 0)
		throw std::runtime_error("pipe() failed.");
	// fork
	pid_t pid = fork();
	if (pid < 0)
		throw std::runtime_error("fork() failed.");
	if (!pid)
	{
		// use pipes
		dup2(pipein[0], 0);
		dup2(pipeout[1], 1);
		close(pipein[1]);
		close(pipeout[0]);
		// arguments to c strings
		char const* const* argv_c = vector_to_c_array(argv);
		char const* const* envp_c = map_to_c_array(envp);
		execve(argv[0].c_str(),
				const_cast<char**>(argv_c),
				const_cast<char**>(envp_c)
			);
		delete[] argv_c;
		delete[] envp_c;
		throw std::runtime_error("execve() failed.");
	}

	// use pipes
	inout[0] = pipeout[0];
	inout[1] = pipein[1];
	close(pipein[0]);
	close(pipeout[1]);
	return pid;
}

void utils::lower(std::string& str)
{
	std::transform(str.begin(), str.end(), str.begin(), tolower);
}

void utils::parseHostAndPort(std::string& host, std::string& port,
	std::string const& default_port,
	std::string str
) {
	size_t i(0);

	// read host
	host = "";
	while (i < str.size() && str[i] != ':') {
		host += str[i];
		i++;
	}
	if (i >= str.size()) {
		port = default_port;
		return;
	}
	i++;

	// read port
	port = "";
	for (size_t j = 0; j < 5; j++) {
		if (i >= str.size() || !std::isdigit(str[i]))
			throw std::runtime_error("digit or EOF expected.");
		port += str[i];
	}
	if (i < str.size())
		throw std::runtime_error("EOF expected.");
}

bool utils::startswith(std::string const& str, std::string const& substr)
{
	if (substr.size() > str.size())
		return false;
	return str.substr(0, substr.size()) == substr;
}

bool utils::endswith(std::string const& str, std::string const& substr)
{
	if (substr.size() > str.size())
		return false;
	return str.substr(str.size() - substr.size()) == substr;
}

std::string utils::generateDefaultError(int statusCode)
{
	std::ostringstream	page;
	std::string			statusMessage = HttpResponse::getReason(statusCode);

	page << "<!DOCTYPE html>\n"
	<< "<html>\n"
	<< "<head><title>" << statusCode << " " << statusMessage << "</title></head>\n"
	<< "<body>\n"
	<< "<h1>" << statusCode << " " << statusMessage << "</h1>\n"
	<< "<p>The server returned an error: " << statusMessage << ".</p>\n"
	<< "</body>\n"
	<< "</html>\n";

	return page.str();
}

std::string utils::generateAutoIndex(const std::string& path, const std::string& urlPath)
{
    DIR *dir;
    struct dirent *entry;
    std::ostringstream page;

    dir = opendir(path.c_str());
    if (!dir)
		return "403";
	page
		<< "<!DOCTYPE html>"
    	<< "<html><head><title> Index of " << urlPath << " </title></head>"
    	<< "<body><h1>Index of " << urlPath << "</h1><ul>";

    if (urlPath != "/")
        page << "<li><a href=\"../\">../</a></li>";

    while ((entry = readdir(dir)) != NULL)
	{
        std::string name = entry->d_name;
        if (name == "." || name == "..")
            continue;

        std::string fullPath = path + "/" + name;
        struct stat st;
        if (stat(fullPath.c_str(), &st) == 0) {
            if (S_ISDIR(st.st_mode))
                name += "/";
        }

        page << "<li><a href=\"" << name << "\">" << name << "</a></li>";
    }

    closedir(dir);

    page << "</ul></body></html>";
    return page.str();
}

std::string	utils::regexWildcardGenerator(const std::string &path)
{
	std::string	regex;

	for (size_t i = 0; i < path.size(); i++)
	{
		if (path[i] == '*')
			regex += "[a-zA-Z0-9\\-_\\.\\%]*";
		else if (path[i] == '.')
			regex += "\\.";
		else if (path[i] == '/')
			regex += "\\/";
		else 
			regex += path[i];
	}
	return "^" + regex + "$";
}

bool	utils::isValidPath(std::string str, std::string path)
{
	std::string pattern = utils::regexWildcardGenerator(path);
	return (utils::isValidRegex(str, pattern));
}

std::string utils::getHeaderParam(const std::string& header, const std::string& key)
{
	size_t start = header.find(key + "=\"");
	if (start == std::string::npos)
		return "";
	start += key.length() + 2;
	size_t end = header.find("\"", start);
	if (end == std::string::npos)
		return "";
	return header.substr(start, end - start);
}

std::string	utils::readFD(int fd, size_t limit)
{
	std::string content;
	size_t size = 0;
	char buffer[1024];
	int bytes;

	while (size < limit)
	{
		bytes = ::read(fd, buffer, sizeof(buffer));
		if (bytes <= 0)
			break ;
		size += bytes;
		content.append(buffer, bytes);
	}
	return (content);
}
