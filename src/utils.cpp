#include "utils.hpp"

#include <algorithm>
#include <stdexcept>
#include <map>
#include <string>
#include <vector>
#include <cstring>
#include <sstream>

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

void forkexec(int inout[2], std::vector<std::string> const argv, std::map<std::string, std::string> const envp)
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
}

std::string int_to_string(int i)
{
	std::stringstream ss;
	ss << i;
	return ss.str();
}

void lower(std::string& str)
{
	std::transform(str.begin(), str.end(), str.begin(), tolower);
}