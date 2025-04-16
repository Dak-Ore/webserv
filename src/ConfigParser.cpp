#include "ConfigParser.hpp"

// Default Constructor
ConfigParser::ConfigParser(File config) {
	std::string content;
	std::string	locationContent;
	int		inServer = 0;
	int		inLocation = 0;
	bool	waitBrace = false;
	std::string line;
	std::vector<std::string> location;
	this->_nbserv = 0;

	for (size_t i = 0; i < config.GetSize(); i++)
	{
		if (inServer > 1 || inLocation > 1)
			throw std::runtime_error("Server in server or location in location");
		line = trim(config.getLine(i));
		if (line.empty() || line[0] == '#')
			continue;
		if (line == "server")
		{
			waitBrace = true;
			continue;
		}
		if (waitBrace)
		{
			if (line == "{")
			{
				inServer++;
				waitBrace = false;
				continue;
			}
		}
		if (line == "server {")
		{
			inServer++;
			continue;
		}
		if (line.find("location") != std::string::npos && line.find("{") != std::string::npos)
			inLocation++;
		if (line == "}")
		{
			if (inLocation)
			{
				inLocation--;
				location.push_back(locationContent);
				locationContent = "";
				continue;
			}
			if (inServer)
			{
				inServer --;
				AddServer(content,location);
				this->_nbserv++;
				location.clear();
				content = "";
				continue;
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

std::string trim(const std::string& s)
{
	size_t start = 0;
	while (start < s.length() && std::isspace(s[start]))
		start++;

	size_t end = s.length();
	while (end > start && std::isspace(s[end - 1]))
		end--;

	return s.substr(start, end - start);
}