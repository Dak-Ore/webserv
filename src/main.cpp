#include <iostream>
#include <string>
#include <csignal>
#include "Server.hpp"

Server* g_server = NULL;

void signalHandler(int signum)
{
	if (signum == SIGINT && g_server != NULL)
	{
		g_server->stop();
		std::cout << "Stopping server ..." << std::endl;
	}
}

int main(int argc, char **argv, char **envp)
{
	(void)envp;
	std::string config_file_name = (argc == 2) ? argv[1] : "";

	try
	{
		Server server("localhost", "8080");
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
