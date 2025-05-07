#include "CGI.hpp"

#include <cctype>
#include <string>
#include <unistd.h>
#include <stdexcept>
#include <map>
#include <cstdlib>
#include "utils.hpp"
#include "const.hpp"

#define CGI_BUFFER_SIZE 1000

static void skip_spc(std::string const& str, size_t &i)
{
	while (i < str.size() && str[i] == ' ')
		i++;
}

static CGI::Running::ResponseHead parse_head(std::string const& head)
{
	CGI::Running::ResponseHead r;
	r.status_code = 200;
	r.status_reason = "OK";

	size_t i = 0;
	while (i < head.size()) {
		// read field name
		std::string name("");
		while (i < head.size() && head[i] != ':') {
			name += head[i];
			i++;
		}
		if (name.size() == 0)
			throw std::runtime_error("field name expected.");
		utils::lower(name);
		i++;
		skip_spc(head, i);

		if (name == "status") {
			// read status code
			std::string status("");
			for (size_t j = 0; j < 3; j++) {
				if (i >= head.size() || !isdigit(head[i]))
					throw std::runtime_error("status code expected.");
				status += head[i];
				i++;
			}
			r.status_code = atoi(status.c_str());
			skip_spc(head, i);

			// read status reason
			std::string reason("");
			while (i < head.size() && head[i] != '\n' && head[i] != '\r') {
				reason += head[i];
				i++;
			}
			r.status_reason = reason;
		}
		else {
			// read field value
			std::string value("");
			while (i < head.size() && head[i] != '\n' && head[i] != '\r') {
				value += head[i];
				i++;
			}
			if (value != "")
				r.fields[name] = value;
		}
	}
	return r;
}

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

CGI::Running CGI::_execute(int& stdin, CGI::execute_arguments const& args)
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
		envp["CONTENT_LENGTH"] = utils::numToString(args.content_length).c_str();
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
	utils::forkexec(inout, argv, envp);
	stdin = inout[1];
	return CGI::Running(inout[0]);
}

CGI::Running CGI::execute(int& stdin,
	std::string const& script,
	std::string const& script_name,
	HttpRequest const& request
) {
	std::string host, port;
	size_t content_length(atoi(request.getHeader("content-length").c_str()));
	utils::parseHostAndPort(host, port, "80", request.getHeader("host"));
	CGI::execute_arguments args = {
		.script_pathname = script,
		.remote_addr = "TODO",
		.request_method = "TODO",
		.script_name = script_name,
		.server_name = host,
		.server_port = port,
		.server_protocol = "HTTP/1.1",
		.query_string = "", // TODO!!
		.path_info = "",
		.content_exists = true ? content_length != 0 : false,
		.content_length = content_length,
		.content_type = request.getHeader("content-type"),
	};
	return this->_execute(stdin, args);
}


CGI::Running::Running()
{}

CGI::Running::Running(CGI::Running const& other)
{
	*this = other;
}

CGI::Running& CGI::Running::operator=(CGI::Running const& other)
{
	this->_stdout = dup(other._stdout);
	this->_complete = other._complete;
	this->_head = other._head;
	this->_head_complete = other._head_complete;
	this->_head_parsed = other._head_parsed;
	this->_response_body_pipe_open = other._response_body_pipe_open;
	if (other._response_body_pipe_open) {
		this->_response_body_pipe[0] = dup(other._response_body_pipe[0]);
		this->_response_body_pipe[1] = dup(other._response_body_pipe[1]);
	}
	return *this;
}

CGI::Running::~Running()
{
	close(this->_stdout);
	if (this->_response_body_pipe_open)
		close(this->_response_body_pipe[1]);
}

CGI::Running::Running(int stdout)
: _stdout(stdout)
, _complete(false)
, _head("")
, _head_complete(false)
, _response_body_pipe_open(false)
{}

bool CGI::Running::read()
{
	char buf[CGI_BUFFER_SIZE];
	ssize_t len = ::read(this->_stdout, buf, CGI_BUFFER_SIZE);
	if (len < 0)
		throw std::runtime_error("read() failed.");

	if (!len) {
		// EOF = there is nothing more to read
		if (!this->_head_complete)
			throw std::runtime_error("the CGI stdout reached EOF before the "
				"head was completely sent.");
		if (this->_response_body_pipe_open) {
			close(this->_response_body_pipe[1]);
			this->_response_body_pipe_open = false;
		}
		this->_complete = true;
		return false;
	}

	if (!this->isHeadComplete()) {
		// head not yet completed
		std::string& head = this->_head;

		head += std::string(buf, len);

		size_t sep;
		size_t bodystart;
		{
			size_t sep1 = head.find("\n\n");
			size_t sep2 = head.find("\r\n\r\n");
			size_t sep3 = head.find("\r\r");
			sep = std::min(std::min(sep1, sep2), sep3);
			bodystart = sep == sep2 ? sep + 4 : sep + 2;
		}

		if (sep != static_cast<size_t>(-1)) {
			// end of head reached

			if (pipe(this->_response_body_pipe) < 0)
				throw std::runtime_error("pipe() failed.");
			this->_response_body_pipe_open = true;
			std::string body(head.substr(bodystart));
			write(this->_response_body_pipe[1], body.c_str(), body.size());

			head = head.substr(0, sep) + "\n";

			this->_head_complete = true;
			this->_head_parsed = parse_head(this->_head);
			return true;
		}

		// end of head not yet reached
		return true;
	}

	// head completed
	write(this->_response_body_pipe[1], buf, len);
	return true;
}

bool CGI::Running::isHeadComplete()
{
	return this->_head_complete;
}

CGI::Running::ResponseHead const CGI::Running::getResponseHead()
{
	return this->_head_parsed;
}

int CGI::Running::getResponseBodyFd()
{
	return this->_response_body_pipe[0];
}

bool CGI::Running::isComplete()
{
	return this->_complete;
}