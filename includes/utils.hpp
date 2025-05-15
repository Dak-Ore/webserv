#pragma once

#include <string>
#include <vector>
#include <map>
#include <regex.h>
#include <sys/types.h> 
#include <stdint.h>
#include <iostream>
#include "HttpResponse.hpp"

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

	std::string generateDefaultError(int statusCode);

	std::string generateAutoIndex(const std::string& path, const std::string& urlPath);
	
	std::string	regexWildcardGenerator(const std::string &path);

	bool	isValidPath(std::string str, std::string path);

	/**
	 * Get the host and port from 'str'.
	 * 
	 * Syntax: host-and-port :== *char [ ":" 1*5digit ]
	 *
	 * 'host' and 'port' are set to the result.
	 * 
	 * If the port is not given, defaults to 'default_port'.
	 */
	void parseHostAndPort(std::string& host, std::string& port,
		std::string const& default_port,
		std::string str
	);

	/**
	 * Return either the start of 'str' is 'substr'.
	 */
	bool startswith(std::string const& str, std::string const& substr);

	/**
	 * Return either the end of 'str' is 'substr'.
	 */
	bool endswith(std::string const& str, std::string const& substr);

	/**
	 * Create a subprocess from 'argv'.
	 * 
	 * - inout[0] and inout[1] will be set to two open file descriptors
	 *   respectively the subprocess' stdout and stdin.
	 * - argv are the arguments that will be given to the subprocess.
	 *   argv[0] must be a path to the executable to call.
	 * - envp is the environment to the subprocess.
	 */
	void forkexec(int inout[2], std::vector<std::string> const argv, std::map<std::string, std::string> const envp);

	/**
	 * Make the given string lowercase.
	 */
	void lower(std::string& str);
};
