#pragma once

#include <string>
#include <vector>
#include <map>
#include <sys/types.h>

class HttpClient;

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
		int getFd() const;

	private:
		/**
		 * stdout is a reading file descriptor linked
		 * to the stdout of the CGI script.
		 */
		Running(int stdout, pid_t pid);

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
		bool isHeadComplete() const;

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
		int getResponseBodyFd() const;

		/**
		 * Return either the CGI script has sent the complete head and body.
		 */
		bool isComplete();

	private:
		pid_t _pid;
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

public:
	CGI(CGI const&);
	CGI& operator=(CGI const&);

	/**
	 * - 'argv': arguments to give to the executable.
	 *           argv[0] must be a path to the executable file
	 *           either absolute or relative to the current pwd (when calling
	 *           this constructor).
	 *           If any is "%f", it will be replaced by the script filename
	 *           when calling .execute().
	 * 
	 * - 'extension': the file extension of this CGI's scripts
	 *                ("php", "py"... without the '.')
	 */
	CGI(std::vector<std::string> const& argv, std::string const& extension);

	virtual ~CGI();

	/**
	 * call the CGI with a given file and other informations.
	 *
	 * The return value is the link to the running CGI script
	 * that must be used to get its response.
	 * 
	 * 'script_pathname' is the path of the script to call.
	 * It should be either absolute or relative to the current pwd.
	 * 
	 * 'script_name' is the URI path of the script.
	 *
	 * 'stdin' will be set to an fd to send the CGI script
	 * potential request data. If it isn't used, it must be closed anyway.
	 *
	 * 'request' is the client's request.
	 * 
	 * This does not check if the file name ends with the CGI's extension.
	 * Use this.fileForMe().
	 */
	CGI::Running *execute(std::string const& script_name, HttpClient const& client) const;

	/**
	 * Checks if the given file name is a the name of a file
	 * you can give to this CGI.
	 * 
	 * This checks if the file extension of 'filename' is this CGI's extension.
	 */
	bool fileForMe(std::string const& filename) const;

private:
	std::vector<std::string> argv;
	std::string extension;
};
