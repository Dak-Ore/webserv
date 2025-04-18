#include "utils.hpp"

size_t my_strlen(char const* const str)
{
	size_t r = 0;
	while (str[r])
		r++;
	return r;
}

char* my_strdup(char const* const str)
{
	char* r = new char[my_strlen(str) + 1];
	size_t i;
	for (i = 0; str[i]; i++)
		r[i] = str[i];
	r[i] = '\0';
	return r;
}

char* concat(char const* const a, char const* const b)
{
	char* r = new char[my_strlen(a) + my_strlen(b) + 1];
	size_t i = 0;
	for (size_t j = 0; a[j]; j++) {
		r[i] = a[j];
		i++;
	}
	for (size_t j = 0; b[j]; j++) {
		r[i] = b[j];
		i++;
	}
	r[i] = '\0';
	return r;
}

char** strarraydup(char const* const array[])
{
	size_t size = 0;
	while (array[size])
		size++;
	char** r = new char*[size + 1];
	size_t i;
	for (i = 0; i < size; i++)
		r[i] = my_strdup(array[i]);
	r[i] = NULL;
	return r;
}

void forkexec(int inout[2], char* const argv[], char* const envp[])
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

		// execute
		// TODO replace /usr/bin/env?
		execve("/usr/bin/env", argv, envp);
		throw std::runtime_error("execve() failed.");
	}

	// use pipes
	inout[0] = pipeout[0];
	inout[1] = pipein[1];
	close(pipein[0]);
	close(pipeout[1]);
}
