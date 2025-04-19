#include <iostream>
#include <string>
#include <csignal>
#include "Webserv.hpp"
#include "ConfigParser.hpp"

Webserv* g_server = NULL;

void signalHandler(int signum)
{
	if (signum == SIGINT && g_server != NULL)
	{
		g_server->stop();
		std::cout << std::endl << "Stopping server ..." << std::endl;
	}
}

int main(int argc, char **argv, char **envp)
{
	(void)envp;
	if (argc != 2)
	{
		std::cout << "Usage " << argv[0] << " <file.conf>" << std::endl;
		return 1;
	}

	std::string config_file_name = (argc == 2) ? argv[1] : "";
	try
	{
		ConfigParser parser(config_file_name);
		Webserv server(parser);
		// std::cout << "Server launched on " << config.getHost()[0] << ":" << config.getPorts()[0] << std::endl;
		g_server = &server;
		signal(SIGINT, signalHandler);
		server.listen();
	}
	catch (const std::exception &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}

	return 0;
}
