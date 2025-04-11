#include <iostream>
#include <string>
#include "File.hpp"

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
		File test = config_file_name;
		std::cout << test.getLine(0);
		std::cout << test.getContent();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	

	
}