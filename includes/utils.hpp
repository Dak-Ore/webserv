#pragma once

#include <unistd.h>
#include <map>
#include <vector>
#include <string>

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
 * Convert an int to a str::string.
 */
std::string int_to_string(int i);

/**
 * Make the given string lowercase.
 */
void lower(std::string& str);