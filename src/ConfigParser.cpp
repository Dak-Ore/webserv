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
				ServerConfig server(content, location);
				std::cout << server.getHost()[0] << ":" << server.getPorts()[0] << std::endl;
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