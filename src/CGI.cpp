#include "CGI.hpp"

#include <exception>
#include <string>
#include <unistd.h>
#include <stdexcept>
#include "utils.hpp"
#include "const.hpp"

CGI::CGI()
{}

CGI::CGI(CGI const& other)
{
	*this = other;
}

CGI& CGI::operator=(CGI const& other)
{
	this->pathname = other.pathname;
	this->argv = other.argv;
	return *this;
}

CGI::CGI(char const* pathname, char const* const argv[])
: pathname(pathname), argv(strarraydup(argv))
{}

CGI::~CGI()
{
	for (size_t i = 0; this->argv[i]; i++)
		delete[] this->argv[i];
	delete[] this->argv;
}

void CGI::execute(int inout[2],
	std::string const& remote_addr,
	std::string const& request_method,
	std::string const& script_name,
	std::string const& script_filename, // TODO same as script path?
	std::string const& server_name,
	std::string const& server_port,
	std::string const& server_protocol,
	bool has_content,
	std::string const& query_string,
	size_t content_length, std::string const& content_type)
{
	std::map<char const*, char const*> envp_map;

	envp_map["GATEWAY_INTERFACE"] = "CGI/1.1";
	envp_map["QUERY_STRING"] = query_string.c_str();
	envp_map["REMOTE_ADDR"] = remote_addr.c_str();
	envp_map["REMOTE_HOST"] = remote_addr.c_str();
	envp_map["REQUEST_METHOD"] = request_method.c_str();
	envp_map["SCRIPT_NAME"] = request_method.c_str();
	envp_map["SCRIPT_FILENAME"] = request_method.c_str();
	envp_map["SERVER_NAME"] = server_name.c_str();
	envp_map["SERVER_PORT"] = server_port.c_str();
	envp_map["SERVER_PROTOCOL"] = server_protocol.c_str();
	envp_map["SERVER_SOFTWARE"] = SERVER_SOFTWARE;
	if (has_content) {
		envp_map["CONTENT_LENGTH"] = (content_length
			? std::to_string(content_length).c_str()
			: ""
		);
		envp_map["CONTENT_TYPE"] = content_type.c_str();
	}
	char** envp = new char*[envp_map.size() + 1];
	size_t i = 0;
	for (
		std::map<char const*, char const*>::iterator it = envp_map.begin();
		it != envp_map.end();
		it++
	) {
		envp[i] = concat(concat(it->first, "="), it->second);
		i++;
	}
	envp[i] = NULL;
	forkexec(inout, this->argv, envp);
}
