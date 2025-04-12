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
	while (std::getline(stream, line) && line.compare("\r") != 0)
	{
		pos = line.find("\r");
		if (pos != line.npos)
			line = line.substr(0, pos);
		pos = line.find(": ");
		if (pos != line.npos)
			this->headers[line.substr(0, pos)] = line.substr(pos + 2);
	}
	while (std::getline(stream, line))
		this->body = line + "\n";
}

HTTPParser::~HTTPParser(){
};