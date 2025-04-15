#include "LocationConfig.hpp"

// Default Constructor
LocationConfig::LocationConfig(std::string content) {
  	std::istringstream stream(content);
	std::string		line;
	

	if (!(stream >> this->_path))
		throw std::runtime_error("invalid location");
	stream >> this->_path;
	std::cout << this->_path << std::endl;
	std::getline(stream, line);
	while (std::getline(stream, line))
	{

	}
}



// Destructor
LocationConfig::~LocationConfig() {
}
