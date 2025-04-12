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
	if (this->version != "HTTP/1.1")
		throw std::runtime_error("505 HTTP Version not supported");
	while (std::getline(stream, line) && line.compare("\r") != 0)
	{
		pos = line.find("\r");
		if (pos == line.npos)
			throw std::runtime_error("Error: Invalid request");
		line = line.substr(0, pos);
		pos = line.find(": ");
		if (pos != line.npos)
		{
			std::string key = line.substr(0, pos);
			if (this->headers.find(key) != this->headers.end())
				throw std::runtime_error("Duplicate header");
			this->headers[key] = line.substr(pos + 2);
		}
	}
	if (this->headers.find("Host") == this->headers.end())
		throw std::runtime_error("400 Bad request: Host header is missing");
	while (std::getline(stream, line))
		this->body += line + "\n";
	checkHeader();
}

void HTTPParser::checkHeader()
{
	std::string methods[] = {"GET", "POST", "DELETE"};
	int i = 0;
	while (i < 3 && methods[i] != this->method)
		i++;

	switch (i)
	{
		case 0: { // GET
			std::string allowedHeadersGet[] = {"Host", "Connection", "Accept", "User-Agent", "Referer"};
			checkAllowed(allowedHeadersGet, 5);
			break;
		}
		case 1: { // POST
			std::string allowedHeadersPost[] = {"Host", "Connection", "Content-Length", "Content-Type", "User-Agent", "Accept", "Referer"};
			checkAllowed(allowedHeadersPost, 7);
			break;
		}
		case 2: { // DELETE
			std::string allowedHeadersDelete[] = {"Host", "Connection", "User-Agent", "Accept"};
			checkAllowed(allowedHeadersDelete, 4);
			break;
		}
		default:
			throw std::runtime_error("405 Method Not Allowed");
	}
}

void HTTPParser::checkAllowed(std::string allowedHeaders[], size_t allowedCount)
{
	if (this->method == "POST" && (this->headers["Content-Type"] == "" || this->headers["Content-Length"] == ""))
		throw std::runtime_error(this->headers["Content-Length"] == "" ? "411 Length Required" : "400 Bad request");
	for (std::map<std::string, std::string>::iterator it = this->headers.begin(); it != this->headers.end(); ++it)
	{
		bool found = false;
		for (size_t i = 0; i < allowedCount; ++i)
		{
			if (it->first == allowedHeaders[i])
			{
				found = true;
				break;
			}
		}
		if (!found)
			throw std::runtime_error("400 Bad Request: Header \"" + it->first + "\" not allowed in GET request");
	}
}

HTTPParser::~HTTPParser(){
};