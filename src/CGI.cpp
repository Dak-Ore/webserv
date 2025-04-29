#include "CGI.hpp"

#include <exception>
#include <string>
#include <unistd.h>
#include <stdexcept>
#include <map>
#include "utils.hpp"
#include "const.hpp"

#define BUFFER_SIZE 1000

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

void CGI::execute(CGI::execute_arguments const& args)
{
	std::map<std::string, std::string> envp;
	envp["GATEWAY_INTERFACE"] = "CGI/1.1";
	envp["QUERY_STRING"] = args.query_string.c_str();
	envp["REMOTE_ADDR"] = args.remote_addr.c_str();
	envp["REQUEST_METHOD"] = args.request_method.c_str();
	envp["PATH_INFO"] = args.path_info.c_str();
	// TODO(maybe) PATH_TRANSLATED
	envp["SCRIPT_NAME"] = args.script_name.c_str();
	envp["SERVER_NAME"] = args.server_name.c_str();
	envp["SERVER_PORT"] = args.server_port.c_str();
	envp["SERVER_PROTOCOL"] = args.server_protocol.c_str();
	envp["SERVER_SOFTWARE"] = SERVER_SOFTWARE;
	if (args.content_exists) {
		envp["CONTENT_LENGTH"] = int_to_string(args.content_length).c_str();
		envp["CONTENT_TYPE"] = args.content_type.c_str();
	}
	// TODO(maybe) HTTP_*

	// for some reason, this isn't specified in the specification
	// at https://datatracker.ietf.org/doc/html/rfc3875#section-5
	// but is necessary, at least for php-cgi
	envp["SCRIPT_FILENAME"] = args.script_pathname;

	std::vector<std::string> argv(this->argv);
	// TODO cd to the script path
	// see https://datatracker.ietf.org/doc/html/rfc3875#section-7.2

	int inout[2];
	if (pipe(inout) < 0)
		throw std::runtime_error("pipe() failed.");
	forkexec(inout, argv, envp);
	return CGI::Running(inout);
}

CGI::Running::Running(int inout[2])
: _inout(inout)
, _responseHeader_complete(false),
, _responseBody_complete(false),
{}

CGI::Running::~Running()
{
	close(inout[0]);
	close(inout[1]);
}

CGI::Running::getResponseHeader()
{
	return this->_responseHeader ? this->_responseHeader_complete : NULL;
}

CGI::Running::getResponseBody()
{
	return this->_responseBody ? this->_responseBody_complete : NULL;
}

CGI::Running::isComplete()
{
	return this->_responseBody_complete;
}

CGI::Running::read()
{
	// TODO ...
}