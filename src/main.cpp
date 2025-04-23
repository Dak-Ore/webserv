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
	argv.push_back("/bin/php-cgi");
	CGI cgi(argv);

	std::cout << "execute cgi" << std::endl;
	std::string data = "favoritemeal=spaghetti";
	int inout[2];
	{
		CGI::execute_arguments args = {
			script_pathname: "/home/zy/local/tmp/elawesome/index.php",
			remote_addr: "192.168.1.247",
			request_method: "POST",
			script_name: "/elawesome/index.php",
			server_name: "example.com",
			server_port: "1234",
			server_protocol: "HTTP/1.1",
			query_string: "name=zy",
			path_info: "",
			content_exists: true,
			content_length: data.size(),
			content_type: "application/x-www-form-urlencoded",
		};
		cgi.execute(inout, args);
	}
	write(inout[1], data.c_str(), data.size());
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
