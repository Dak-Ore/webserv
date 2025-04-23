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

// TODO(debug) test cgi
int main()
{
	std::cout << "create cgi" << std::endl;
	std::vector<std::string> argv;
	argv.push_back("./testers/ubuntu_cgi_tester");
	argv.push_back("-f");
	CGI cgi(argv);

	std::cout << "execute cgi" << std::endl;
	int inout[2];
	cgi.execute(inout,
			"/home/zy/local/tmp/elawesome/index.php",
			"192.168.1.247",
			"GET",
			"/",
			"/elawesome/index.php",
			"yeyaaaa.example.com",
			"1234",
			"HTTP/1.1",
			"name=zy");
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
