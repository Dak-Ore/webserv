#pragma once

#include <string>
#include <vector>
#include <map>
#include "HttpRequest.hpp"

/**
 * Allows to execute a CGI script with a given file and other informations.
 */
class CGI
{

public:
	/**
	 * Manages a running CGI script. This allows to send and receive data
	 * from the CGI script until its end.
	 *
	 * Return type of CGI.execute().
	 */
	class Running
	{
	public:
		Running();
		Running(Running const& other);
		CGI::Running& operator=(Running const& other);
		virtual ~Running();

	private:
		/**
		 * stdout is a reading file descriptor linked
		 * to the stdout of the CGI script.
		 */
		Running(int stdout);

	public:
		/**
		 * To call each time there is something to read on the CGI script's
		 * stdout or if it reached EOF.
		 *
		 * Must be called repeteadly until this.isComplete().
		 *
		 * Return false when EOF.
		 */
		bool read();

		/**
		 * Return either the CGI script has sent the complete head.
		 */
		bool isHeadComplete();

		struct ResponseHead
		{
			int status_code;
			std::string status_reason;
			std::map<std::string, std::string> fields;
		};

		/**
		 * Get the response head of the CGI script.
		 *
		 * Call it only if this.isHeadComplete() is true!
		 */
		ResponseHead const getResponseHead();

		/**
		 * Get a read-only fd that reads the body content of the CGI script.
		 *
		 * Call it only if this.isHeadComplete() is true!
		 */
		int getResponseBodyFd();

		/**
		 * Return either the CGI script has sent the complete head and body.
		 */
		bool isComplete();

	private:
		int _stdout;
		bool _complete;
		std::string _head;
		bool _head_complete;
		ResponseHead _head_parsed;
		int _response_body_pipe[2];
		bool _response_body_pipe_open;

		friend class CGI;
	};

private:
	CGI();
	CGI(CGI const&);
	CGI& operator=(CGI const&);

public:
	/**
	 * - 'argv': arguments to give to the executable.
	 *           argv[0] must be a path to the executable file.
	 */
	CGI(std::vector<std::string> const& argv);

	virtual ~CGI();

	/**
	 * arguments to give to CGI::execute().
	 * optional arguments can be "".
	 * most of them are meta-arguments defined at
	 * https://datatracker.ietf.org/doc/html/rfc3875#section-4.1
	 */
	struct execute_arguments
	{
		/**
		 * path of the file to give to the CGI.
		 */
		std::string const& script_pathname; 

		/**
		 * defines the meta-variable REMOTE_ADDR:
		 * IPv4 address of the agent sending the request to the server
		 * (probably the client).
		 */
		std::string const& remote_addr;

		/**
		 * defines the meta-argument REQUEST_METHOD:
		 * the http request method (ex: "GET", "POST"...)
		 */
		std::string const& request_method;

		/**
		 * defines the meta-argument SCRIPT_NAME:
		 * URI path of the script to call.
		 *
		 * this is not the same as 'script_path',
		 * http://example.com/foo/bar.php would give "/foo/bar.php",
		 * while script_path could give "/var/www/foo/bar.php"
		 */
		std::string const& script_name;

		/**
		 * defines the meta-argument SERVER_NAME:
		 * domain name or IPv4 of the server,
		 * this can be taken from the URI of the client's request.
		 */
		std::string const& server_name;

		/**
		 * defines the meta-argument SERVER_PORT:
		 * it's the port of the server in decimal lol
		 */
		std::string const& server_port;

		/**
		 * defines the meta-argument SERVER_PROTOCOL:
		 * version of HTTP used to answer the request,
		 * probably "HTTP/1.1".
		 */
		std::string const& server_protocol;

		/**
		 * (optional) defines the meta-argument QUERY_STRING:
		 * in the url https://example.com/foo/bar.php?favorite=spaghetti,
		 * it's "favorite=spaghetti" (without '?').
		 */
		std::string const& query_string;

		/**
		 * (optional) defines the meta-argument PATH_INFO:
		 * see https://datatracker.ietf.org/doc/html/rfc3875#section-4.1.5
		 *
		 * probably not useful, i guess keep it "".
		 */
		std::string const& path_info;

		/**
		 * either the client sent request data (probably with REQUEST_METHOD=POST).
		 *
		 * if this is defined, content must be defined too,
		 * and data must be wrote on inout[1].
		 */
		bool content_exists;

		/**
		 * (optional) defines the meta-argument CONTENT_LENGTH:
		 * size of the request data, in bytes.
		 *
		 * to use if .content_exists is true.
		 */
		size_t content_length;

		/**
		 * (optional) defines the meta-argument CONTENT_LENGTH:
		 * MIME type of the request data.
		 *
		 * to use if .content_exists is true.
		 */
		std::string const& content_type;
	};

	/**
	 * call the CGI with a given file and other informations.
	 *
	 * The return value is the link to the running CGI script
	 * that must be used to get its response.
	 * 
	 * 'script_pathname' is the absolute path of the script to call.
	 * 
	 * 'script_name' is the URI path of the script.
	 *
	 * 'stdin' will be set to an fd to send the CGI script
	 * potential request data. If it isn't used, it must be closed anyway.
	 *
	 * 'request' is the client's request.
	 */
	CGI::Running execute(int& stdin,
		std::string const& script_pathname,
		std::string const& script_name,
		HttpRequest const& request
	);

private:
	CGI::Running _execute(int& stdin, CGI::execute_arguments const& args);

	std::vector<std::string> argv;
};
