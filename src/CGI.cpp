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

CGI::CGI(std::vector<std::string> const& argv)
: argv(argv)
{}

CGI::~CGI()
{}

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
	std::map<std::string, std::string> envp;
	envp["GATEWAY_INTERFACE"] = "CGI/1.1";
	envp["QUERY_STRING"] = query_string.c_str();
	envp["REMOTE_ADDR"] = remote_addr.c_str();
	envp["REQUEST_METHOD"] = request_method.c_str();
	envp["PATH_INFO"] = path_info.c_str();
	envp["SCRIPT_NAME"] = script_name.c_str();
	envp["SERVER_NAME"] = server_name.c_str();
	envp["SERVER_PORT"] = server_port.c_str();
	envp["SERVER_PROTOCOL"] = server_protocol.c_str();
	envp["SERVER_SOFTWARE"] = SERVER_SOFTWARE;
	if (content_type != "") {
		envp["CONTENT_LENGTH"] = (content_length
			? int_to_string(content_length).c_str()
			: ""
		);
		envp["CONTENT_TYPE"] = content_type.c_str();
	}

	std::vector<std::string> argv(this->argv);
	argv.push_back(script_path);

	forkexec(inout, argv, envp);
}
