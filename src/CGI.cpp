#include "CGI.hpp"

#include <exception>
#include <string>
#include <unistd.h>
#include <stdexcept>
#include "utils.hpp"

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

void CGI::execute(int inout[2], std::map<std::string, std::string> metavars, std::string body)
{
	(void)metavars;
	(void)body;
	int pipein[2];
	if (pipe(pipein) < 0)
		throw std::runtime_error("pipe() failed.");
	int pipeout[2];
	if (pipe(pipeout) < 0)
		throw std::runtime_error("pipe() failed.");
	pid_t pid = fork();
	if (pid < 0)
		throw std::runtime_error("fork() failed.");
	if (!pid)
	{
		dup2(pipein[0], 0);
		dup2(pipeout[1], 1);
		close(pipein[1]);
		close(pipeout[0]);
		char* null = NULL;
		execve(this->pathname.c_str(), this->argv, &null);
		throw std::runtime_error("execve() failed.");
	}
	inout[0] = pipeout[0];
	inout[1] = pipein[1];
	close(pipein[0]);
	close(pipeout[1]);
}