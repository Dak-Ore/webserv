#include <iostream>
#include <string>
#include "File.hpp"
#include "HTTPParser.hpp"

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
	try
	{
		File		content(config_file_name);
		HTTPParser	test("<Method> <Path> <Version>\r\nHeader1: Value1\r\nHeader2: Value2\r\n...\r\n\r\n<Body> (optionnel)");
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	

	
}