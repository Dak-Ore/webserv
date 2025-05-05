#include <iostream>
#include <stdexcept>
#include <string>
#include <unistd.h>
#include "CGI.hpp"

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
	CGI::Running running;
	int stdin;
	{
		CGI::execute_arguments args = {
			.script_pathname = "/home/zy/local/tmp/elawesome/index.php",
			.remote_addr = "192.168.1.247",
			.request_method = "POST",
			.script_name = "/elawesome/index.php",
			.server_name = "example.com",
			.server_port = "1234",
			.server_protocol = "HTTP/1.1",
			.query_string = "name=zy",
			.path_info = "",
			.content_exists = true,
			.content_length = data.size(),
			.content_type = "application/x-www-form-urlencoded",
		};
		running = cgi.execute(stdin, args);
	}

	write(stdin, data.c_str(), data.size());
	close(stdin);

	while (!running.isHeadComplete())
		running.read();
	std::cout << "HEAD" << std::endl;
	CGI::Running::ResponseHead head(running.getResponseHead());
	std::cout << "status code: "<< head.status_code << std::endl;
	std::cout << "status reason: '"<< head.status_reason << "'" << std::endl;
	for (std::map<std::string, std::string>::const_iterator it = head.fields.begin(); it != head.fields.end(); it++)
		std::cout << "'" << it->first << "': '" << it->second << "'" << std::endl;
	std::cout << std::endl;

	while (running.read());

	int bodyfd = running.getResponseBodyFd();
	std::cout << "BODY" << std::endl;
	char buf[2];
	while (ssize_t n = read(bodyfd, buf, 2)) {
		if (n < 0)
			throw std::runtime_error("bruh.");
		write(1, buf, n);
	}

	std::cout << "%" << std::endl << ":)" << std::endl << std::endl;
}
