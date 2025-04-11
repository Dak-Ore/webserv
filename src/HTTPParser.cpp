#include "HTTPParser.hpp"

// Default Constructor
HTTPParser::HTTPParser(std::string request) {
	std::istringstream	stream(request);
	std::string 		line;

	if (request.empty())
		throw std::runtime_error("Error: when trying to read a HTTP request.");
	std::getline(stream, line);
	size_t	pos = line.find("\r");
	if (pos == line.npos)
		throw std::runtime_error("Error: Invalid request");
	line = line.substr(0, pos);
	std::istringstream request_line(line);
	request_line >> this->method >> this->path >> this->version;
	 
}

HTTPParser::~HTTPParser(){
};