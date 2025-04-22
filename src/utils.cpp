#include "utils.hpp"

#include <stdexcept>

void forkexec(int inout[2], const char* const argv[], const char* const envp[])
{
	// create pipes
	int pipein[2];
	if (pipe(pipein) < 0)
		throw std::runtime_error("pipe() failed.");
	int pipeout[2];
	if (pipe(pipeout) < 0)
		throw std::runtime_error("pipe() failed.");

	// fork
	pid_t pid = fork();
	if (pid < 0)
		throw std::runtime_error("fork() failed.");
	if (!pid)
	{
		// use pipes
		dup2(pipein[0], 0);
		dup2(pipeout[1], 1);
		close(pipein[1]);
		close(pipeout[0]);
		execve(argv[0],
				const_cast<char* const*>(argv),
				const_cast<char* const*>(envp)
			);
		throw std::runtime_error("execve() failed.");
	}

	// use pipes
	inout[0] = pipeout[0];
	inout[1] = pipein[1];
	close(pipein[0]);
	close(pipeout[1]);
}
