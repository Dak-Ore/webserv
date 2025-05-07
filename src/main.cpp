#include <iostream>
#include <stdexcept>
#include <string>
#include <csignal>
#include "Webserv.hpp"
#include "ConfigParser.hpp"

Webserv* g_server = NULL;

/**
 * Handle SIGINT.
 */
static void signalHandler(int signum)
{
	if (signum == SIGINT && g_server != NULL)
	{
		g_server->stop();
		std::cout << std::endl << "Stopping server ..." << std::endl;
	}
}


int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "Usage " << argv[0] << " <file.conf>" << std::endl;
		return 1;
	}

	std::string config_file_name = argv[1];
	if (!utils::endswith(config_file_name, ".conf"))
		throw std::runtime_error("Config file must have extension '.conf'.");
	ConfigParser parser(config_file_name);
	Webserv server(parser);
	g_server = &server;
	signal(SIGINT, signalHandler);
	server.listen();

	return 0;
}
