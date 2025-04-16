#include <iostream>
#include <string>
#include "File.hpp"
#include "HTTPParser.hpp"
#include "Server.hpp"
#include "ConfigParser.hpp"

int main(int argc, char **argv, char **envp)
{
	(void)envp;
	try
	{
		std::string t = argv[argc - 1];
		ConfigParser config(t);
		std::vector<ServerConfig> configlst = config.getServer();
		for (size_t i = 0; i < config.getNbServ(); i++)
			configlst[i].print();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
	// if (argc > 2)
	// {
	// 	std::cerr << "Usage: " << argv[0] << "[configuration file]" << std::endl;
	// 	return (2);
	// }
	// std::string config_file_name = (argc == 2) ? argv[1] : "default";
	// std::cout << config_file_name << std::endl;
	// try
	// {
	// 	File		content(config_file_name);
	// 	HTTPParser	test(content.getContent());
	// 	std::cout << "here" << std::endl;
	// }
	// catch(const std::exception& e)
	// {
	// 	std::cerr << e.what() << '\n';
	// }
	// try
	// {
	// 	HTTPParser	test("GET / HTTP/2.0\r\nHost: localhost\r\n\r\n");
	// }
	// catch(const std::exception& e)
	// {
	// 	std::cerr << e.what() << '\n';
	// }
	// try
	// {
	// 	HTTPParser	test("GET / HTTP/1.1\r\nHost: localhost\r\nContent-Length: 10\r\n\r\n");
	// }
	// catch(const std::exception& e)
	// {
	// 	std::cerr << e.what() << '\n';
	// }
	// try
	// {
	// 	HTTPParser	test("POST /submit HTTP/1.1\r\nHost: localhost\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\nname=bob&age=42");
	// }
	// catch(const std::exception& e)
	// {
	// 	std::cerr << e.what() << '\n';
	// }
	
}