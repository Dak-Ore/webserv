#include "ConfigParser.hpp"

// Default Constructor
ConfigParser::ConfigParser(File config) {
	std::string content;
	int	nbsub = 0;
	for (size_t i = 0; i < config.GetSize(); i++)
	{
		size_t pos = config.getLine(i).find("server");
		if (pos != std::string::npos)
		{
			pos = config.getLine(i).find("{");
			if (pos != std::string::npos)
			{

			}
			else
			{
				i++;
				pos = config.getLine(i).find("{");
				if (pos == std::string::npos)
					throw std::runtime_error("Invalid config file");
			}
		}
	}
}

// Destructor
ConfigParser::~ConfigParser() {
    std::cout << "ConfigParser destructor called" << std::endl;
}
