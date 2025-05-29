#include "CGI.hpp"
#include "HttpClient.hpp"
#include <cctype>
#include <string>
#include <unistd.h>
#include <stdexcept>
#include <map>
#include <cstdlib>
#include <climits>
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
	this->extension = other.extension;
	return *this;
}

CGI::CGI(std::vector<std::string> const& argv, std::string const& extension)
: argv(argv)
, extension(extension)
{}

CGI::~CGI()
{}

CGI::Running CGI::execute(int& stdin,
	std::string const& script_pathname,
	std::string const& script_name,
	HttpClient const& client
) const {
	std::string script(realpath(script_pathname.c_str(), NULL));
	HttpRequest const& request = client.request();
	std::string content_length_str(request.getHeader("Content-Length"));
	size_t content_length(utils::stringToNum(content_length_str));

	std::map<std::string, std::string> envp;
	envp["GATEWAY_INTERFACE"] = "CGI/1.1";
	envp["QUERY_STRING"] = request.getRawOpt();
	envp["REMOTE_ADDR"] = client.getAdress().str();
	envp["REQUEST_METHOD"] = request.getMethod();
	envp["PATH_INFO"] = "";
	envp["PATH_TRANSLATED"] = "";
	envp["SCRIPT_NAME"] = script_name;
	envp["SERVER_NAME"] = client.getServerAdress().host_str();
	envp["SERVER_PORT"] = client.getServerAdress().port_str();
	envp["SERVER_PROTOCOL"] = "HTTP/1.1";
	envp["SERVER_SOFTWARE"] = SERVER_SOFTWARE;
	if (content_length != 0) {
		envp["CONTENT_LENGTH"] = content_length_str;
		envp["CONTENT_TYPE"] = request.getHeader("Content-Type");
	}

	// for some reason, this isn't specified in the specification
	// at https://datatracker.ietf.org/doc/html/rfc3875#section-5
	// but is necessary, at least for php-cgi
	envp["SCRIPT_FILENAME"] = script;

	// Récupérer et ajouter les cookies à l'environnement
	std::string cookies = request.getRawCookie();
	if (!cookies.empty()) {
		envp["HTTP_COOKIE"] = cookies;  // Les cookies doivent être envoyés sous la forme "Cookie: ..."
	}

	std::vector<std::string> argv(this->argv);
	int inout[2];
	utils::forkexec(inout, argv, envp);
	stdin = inout[1]; // why ?
	return CGI::Running(inout[0]);
}

bool CGI::fileForMe(std::string const& filename) const
{
	return utils::endswith(filename, utils::trim("." + this->extension));
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

    if (len == 0)
	{
        // Ensure that the header is completely received before handling EOF
        if (!this->_head_complete)
            throw std::runtime_error("CGI stdout reached EOF before the head was completely sent.");
        
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
