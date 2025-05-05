#include <string>
#include <vector>
#include <regex.h>
#include <sys/types.h> 
#include <stdint.h>
#include <iostream>

namespace utils
{
	/**
	 * @brief Convert a size_t number to a string.
	 * @param value The number to convert.
	 * @return String representation of the number.
	 */
	std::string numToString(size_t value);

	/**
	 * @brief Check if a file exists at the given path.
	 * @param path The file path to check.
	 * @return True if the file exists, false otherwise.
	 */
	bool fileExists(const std::string &path);

	/**
	 * @brief Get the size of a file in bytes.
	 * @param path Path to the file.
	 * @return File size in bytes, or -1 if the file doesn't exist.
	 */
	off_t getFileSize(const std::string& path);

	/**
	 * @brief Join a base path and a relative path into a single path.
	 * @param base Base directory.
	 * @param relative Relative path or filename.
	 * @return Combined file path.
	 */
	std::string joinPath(const std::string &base, const std::string &relative);
	std::string smartSubstr(std::string line, std::string start, std::string end);
	std::string trim(const std::string& s);
	std::string extractKey(std::string line);
	void ft_split(std::string line, std::vector<std::string> *list);
	bool isValidRegex(std::string str, std::string pattern); 
	bool isDirectory(const std::string &path);
	/**
	 * @brief Add a trailing slash to a path if it doesn't already have one.
	 * @param path Input path.
	 * @return Path with a trailing slash.
	 */
	std::string addTrailingSlash(const std::string& path);
	/**
	 * @brief Remove the trailing slash from a path if it exists.
	 * @param path Input path.
	 * @return Path without a trailing slash.
	 */
	std::string removeTrailingSlash(const std::string& path);

	std::string extractElem(std::string line, int pos);
}
