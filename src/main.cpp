#include <iostream>
#include <string>
#include "Server.hpp"

#include "Server.hpp"

int main(int argc, char **argv, char **envp)
{
	(void)envp;
	std::string config_file_name = (argc == 2) ? argv[1] : "default";
	std::cout << config_file_name << std::endl;

    try
    {
        Server server("localhost", "8080");
        server.listen();
        std::cout << "Server is now listening for connections..." << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
