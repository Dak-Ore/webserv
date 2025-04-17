#include <iostream>
#include <string>
#include "Server.hpp"
#include "ConfigParser.hpp"
#include "Server.hpp"

int main(int argc, char **argv, char **envp)
{
	(void)envp;
	std::string config_file_name = (argc == 2) ? argv[1] : "default";

    try
    {
		File 		config(config_file_name);
		ConfigParser parser(config);
		ServerConfig servConfig = parser.getServer()[0];
        Server server(servConfig.getHost()[0], servConfig.getPorts()[0]);
        server.listen();
        std::cout << "Server is now listening for connections..." << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
