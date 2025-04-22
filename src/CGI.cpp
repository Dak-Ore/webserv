#include "CGI.hpp"

#include <exception>
#include <string>
#include <unistd.h>
#include <stdexcept>
#include <map>
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
	this->argv = other.argv;
	return *this;
}

CGI::CGI(char const* const argv[])
: argv(arraydupl(argv))
{}

CGI::~CGI()
{
	for (size_t i = 0; this->argv[i]; i++)
		delete[] this->argv[i];
	delete[] this->argv;
}

void CGI::execute(int inout[2],
		std::string const& script_path, // path of the script to give to the cgi
		std::string const& remote_addr, // ipv4 of the agent sending the request to the server
		std::string const& request_method, // "GET", "POST"...
		std::string const& path_info, // path of the client request
		std::string const& script_name, // URI path of the script to call
		std::string const& server_name,	// domain name or ipv4 of the server, this can be taken from the uri of the client's request.
		std::string const& server_port, // port on which the request was received.
		std::string const& server_protocol, // "HTTP/ " *.* (version of HTTP used to answer the request)
		std::string const& query_string,
		size_t content_length, // IF has_content: number of bytes of it
		std::string const& content_type // IF has_content: mime type of it
	)
{
	std::map<char const*, char const*> envp_map;

	envp_map["GATEWAY_INTERFACE"] = "CGI/1.1";
	envp_map["QUERY_STRING"] = query_string.c_str();
	envp_map["REMOTE_ADDR"] = remote_addr.c_str();
	envp_map["REQUEST_METHOD"] = request_method.c_str();
	envp_map["PATH_INFO"] = path_info.c_str();
	envp_map["SCRIPT_NAME"] = script_name.c_str();
	envp_map["SERVER_NAME"] = server_name.c_str();
	envp_map["SERVER_PORT"] = server_port.c_str();
	envp_map["SERVER_PROTOCOL"] = server_protocol.c_str();
	envp_map["SERVER_SOFTWARE"] = SERVER_SOFTWARE;
	if (content_type != "") {
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
	char const* const argvend[] = {script_path.c_str(), NULL};
	forkexec(inout, concat(this->argv, argvend), envp);
}
