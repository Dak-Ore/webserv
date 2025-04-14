#include "HTTPParser.hpp"

// Default Constructor
HTTPParser::HTTPParser(std::string request) {
	if (request.empty())
		throw std::runtime_error("Error: when trying to read a HTTP request.");

	std::istringstream stream(request);

	parseRequestLine(stream);
	parseHeaders(stream);

	if (this->headers.size() > MAX_HEADERS)
		throw std::runtime_error("431 Request Header Fields Too Large");

	parseBody(stream);
	if (method == "POST")
		validateBodySize();
}

void HTTPParser::parseRequestLine(std::istringstream& stream) {
	std::string line;
	std::getline(stream, line);
	size_t pos = line.find("\r");
	if (pos != std::string::npos)
		line = line.substr(0, pos);
		// throw std::runtime_error("400 Bad Request: Invalid request line");

	std::istringstream request_line(line);
	if (!(request_line >> this->method >> this->path >> this->version))
		throw std::runtime_error("400 Bad Request: Malformed request line");

	if (this->version != "HTTP/1.1")
		throw std::runtime_error("505 HTTP Version Not Supported");
}

void HTTPParser::parseHeaders(std::istringstream& stream) {
	std::string line;
	while (std::getline(stream, line) && line != "\r") {
		size_t pos = line.find("\r");
		if (pos != std::string::npos)
			line = line.substr(0, pos);
			// throw std::runtime_error("400 Bad Request: Invalid header line");
		pos = line.find(": ");
		if (pos != std::string::npos) {
			std::string key = line.substr(0, pos);
			if (headers.find(key) != headers.end())
				throw std::runtime_error("400 Bad Request: Duplicate header");
			headers[key] = line.substr(pos + 2);
		}
	}

	if (headers.find("Host") == headers.end() || headers["Host"].empty() )
		throw std::runtime_error("400 Bad Request");
}

void HTTPParser::parseBody(std::istringstream& stream) {
	std::string line;
	while (std::getline(stream, line))
		this->body += line + "\n";
}

void HTTPParser::validateBodySize() {
	std::map<std::string, std::string>::iterator len = headers.find("Content-Length");
	if (len == headers.end() || len->second.empty())
		throw std::runtime_error("411 Length Required");

	size_t expectedLength = std::strtoul(len->second.c_str(), NULL, 10);
	size_t actualLength = body.size();

	if (actualLength != expectedLength)
		throw std::runtime_error("400 Bad Request: Body size does not match Content-Length");
}

void HTTPParser::print()
{
	std::cout << "Request: " << std::endl;
	std::cout << this->method << " - " << this->path << " - " << this->version << std::endl;
	std::cout << "Headers :" << std::endl;
	for (std::map<std::string, std::string>::iterator i = this->headers.begin(); i != this->headers.end(); i++)
		std::cout << i->first << " - " << i->second << std::endl;
}
std::string	HTTPParser::getMethod()
{
	return (this->method);
}
std::string	HTTPParser::getPath()
{
	return (this->path);
}
std::string	HTTPParser::getVersion()
{
	return (this->version);
}
std::map<std::string, std::string>	HTTPParser::getHeaders()
{
	return (this->headers);
}
std::string	HTTPParser::getBody()
{
	return (this->body);
}

// void HTTPParser::checkHeader()
// {
// 	std::string methods[] = {"GET", "POST", "DELETE"};
// 	int i = 0;
// 	while (i < 3 && methods[i] != this->method)
// 		i++;

// 	switch (i)
// 	{
// 		case 0: { // GET
// 			std::string allowedHeadersGet[] = {"Host", "Connection", "Accept", "User-Agent", "Referer"};
// 			checkAllowed(allowedHeadersGet, 5);
// 			break;
// 		}
// 		case 1: { // POST
// 			std::string allowedHeadersPost[] = {"Host", "Connection", "Content-Length", "Content-Type", "User-Agent", "Accept", "Referer"};
// 			checkAllowed(allowedHeadersPost, 7);
// 			break;
// 		}
// 		case 2: { // DELETE
// 			std::string allowedHeadersDelete[] = {"Host", "Connection", "User-Agent", "Accept"};
// 			checkAllowed(allowedHeadersDelete, 4);
// 			break;
// 		}
// 		default:
// 			throw std::runtime_error("405 Method Not Allowed");
// 	}
// }

// void HTTPParser::checkAllowed(std::string allowedHeaders[], size_t allowedCount)
// {
// 	for (std::map<std::string, std::string>::iterator it = this->headers.begin(); it != this->headers.end(); ++it)
// 	{
// 		bool found = false;
// 		for (size_t i = 0; i < allowedCount; ++i)
// 		{
// 			if (it->first == allowedHeaders[i])
// 			{
// 				found = true;
// 				break;
// 			}
// 		}
// 		if (!found)
// 			throw std::runtime_error("400 Bad Request: Header \"" + it->first + "\" not allowed in " + this->method + " request");
// 	}
// }

HTTPParser::~HTTPParser(){
};