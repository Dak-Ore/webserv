#pragma once

#include <string>
#include <vector>

/**
 * Allows to execute a CGI script with a given file and other informations.
 * 
 * TODO must work with poll
 */
class CGI
{

private:
	/**
	 * Represents the response header of the CGI script.
	 */
	struct ResponseHeader
	{};

	/**
	 * Manages a running CGI script. This allows to send and receive data
	 * from the CGI script until its end.
	 * 
	 * Return type of CGI.execute().
	 */
	class Running
	{

	public:
		/**
		 * inout[0] and inout[1] are respectively
		 * a reading file descriptor linked to the stdout of the CGI script
		 * and a writing file descriptor linked to its stdin.
		 */
		Running(int inout[2]);

		virtual ~Running();

		/**
		 * To call each time there is something to read on the CGI script's stdout.
		 */
		void read();

		/**
		 * Get the reponse header of the CGI script.
		 * If it wasn't given by the CGI script yet, return NULL.
		 */
		CGI::ResponseHeader const* getResponseHeader();

		/**
		 * Get the reponse body of the CGI script.
		 * If it wasn't given by the CGI script yet, return NULL.
		 */
		CGI::ResponseHeader const* getResponseBody();

		/**
		 * Either the CGI script is finished.
		 */
		bool isComplete();

	private:
		int _inout[2];
		CGI::ResponseHeader _responseHeader;
		bool _responseHeader_complete;
		std::string _responseBody;
		bool _responseBody_complete;

	};

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
	 * that must be used to get its response, and to send it
	 * potential data.
	 */
	CGI::Running execute(CGI::execute_arguments const& args);

private:
	std::vector<std::string> argv;
};
