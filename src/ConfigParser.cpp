#include "ConfigParser.hpp"
#include "utils.hpp"

ConfigParser::ConfigParser(File config) {
	std::string content;
	std::string	locationContent;
	int		inServer = 0;
	int		inLocation = 0;
	int		waitBrace = 0;
	std::string line;
	std::vector<std::string> location;
	this->_nbserv = 0;

	for (size_t i = 0; i < config.GetSize(); i++)
	{
		if (inServer > 1 || inLocation > 1)
			throw std::runtime_error("Server in server or location in location");
		line = utils::trim(config.getLine(i));
		if (line.empty() || line[0] == '#')
			continue;
		if (line == "server")
		{
			waitBrace = 1;
			continue;
		}
		if (line.find("location") != std::string::npos && line.find("{") == std::string::npos)
		{
			waitBrace = 2;
			continue;
		}
		if (waitBrace && line == "{")
		{
			if (waitBrace == 1)
				inServer++;
			else
				inLocation++;
			waitBrace = false;
			continue;
		}
		else
		if (line == "server {")
		{
			inServer++;
			continue;
		}
		if (line.find("location") != std::string::npos && line.find("{") != std::string::npos)
			inLocation++;
		if (waitBrace || line == "{")
			throw std::runtime_error("Invalid config file");
		if (line == "}")
		{
			if (inLocation)
			{
				inLocation--;
				location.push_back(locationContent);
				locationContent = "";
				continue;
			}
			else if (inServer)
			{
				inServer --;
				AddServer(content,location);
				this->_nbserv++;
				location.clear();
				content = "";
				continue;
			}
			else
			{
				
			}
		}
		if (inLocation == 0)
			content += line + '\n';
		else
			locationContent += line + '\n';
	}
}

// Destructor
ConfigParser::~ConfigParser() {
}

void	ConfigParser::AddServer(std::string content, std::vector<std::string> location)
{
	ServerConfig newServ(content, location);
	this->_server.push_back(newServ);
}

std::vector<ServerConfig>	ConfigParser::getServer(){return this->_server;}
size_t			ConfigParser::getNbServ(){return this->_nbserv;}
