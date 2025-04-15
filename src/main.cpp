#include <iostream>
#include <stdexcept>
#include <string>
#include "CGI.hpp"
#include "utils.hpp"

/*
int main(int argc, char **argv, char **envp)
{
	(void)envp;
	if (argc > 2)
	{
		std::cerr << "Usage: " << argv[0] << "[configuration file]" << std::endl;
		return (2);
	}
	std::string config_file_name = (argc == 2) ? argv[1] : "default";
	std::cout << config_file_name << std::endl;
}*/

int main()
{
	std::cout << "create cgi" << std::endl;
	char const* argv[] = {"/bin/cat", NULL};
	CGI cgi("/bin/rev", argv);
	std::cout << "execute cgi" << std::endl;
	int inout[2];
	cgi.execute(inout, std::map<std::string, std::string>(), "");
	write(inout[1], "hello\nworld\n!\n:3\n", my_strlen("hello\nworld\n!\n:3\n"));
	close(inout[1]);

	char buf[1000];

	while (true) {
		int n = read(inout[0], buf, 1000);
		if (n < 0)
			throw std::runtime_error("read() failed.");
		if (!n)
			break;
		write(1, buf, n);
	}
	close(inout[0]);

	std::cout << std::endl << ":)" << std::endl;
}