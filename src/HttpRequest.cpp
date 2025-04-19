#include "HttpRequest.hpp"

HttpRequest::HttpRequest() : HttpMessage() {}
// Default Constructor
HttpRequest::HttpRequest(const std::string &request) : HttpMessage(),
	_is_empty(request.empty()), _error(0)
{
	if (this->empty())
		return ;

	std::istringstream stream(request);

	parseRequestLine(stream);
	parseHeaders(stream);

	if (this->_headers.size() > MAX_HEADERS)
	{
		this->_error = 431;
		return ;
	}
	parseBody(stream);
	if (_method == "POST")
		validateBodySize();
}

void HttpRequest::parseRequestLine(std::istringstream& stream)
{
	std::string line;
	std::getline(stream, line);
	size_t pos = line.find("\r");
	if (pos != std::string::npos)
		line = line.substr(0, pos);
		// throw std::runtime_error("400 Bad Request: Invalid request line");

	std::istringstream request_line(line);
	if (!(request_line >> this->_method >> this->_path >> this->_version))
	{
		this->_error = 400;
		return ;
	}

	if (this->_version != "HTTP/1.1")
		throw std::runtime_error("505 HTTP Version Not Supported");
}

void HttpRequest::parseHeaders(std::istringstream& stream)
{
	std::string line;
	while (std::getline(stream, line) && line != "\r") {
		size_t pos = line.find("\r");
		if (pos != std::string::npos)
			line = line.substr(0, pos);
		pos = line.find(": ");
		if (pos != std::string::npos) {
			std::string key = line.substr(0, pos);
			if (this->_headers.find(key) != this->_headers.end())
			{
				this->_error = 400;
				return ;
			}
			this->_headers[key] = line.substr(pos + 2);
		}
	}

	if (this->_headers.find("Host") == this->_headers.end() || this->_headers["Host"].empty() )
	{
		this->_error = 400;
		return;
	}
}

void HttpRequest::parseBody(std::istringstream& stream)
{
	std::string line;
	while (std::getline(stream, line))
		this->_body += line + "\n";
}

void HttpRequest::validateBodySize()
{
	std::map<std::string, std::string>::iterator len = this->_headers.find("Content-Length");
	if (len == this->_headers.end() || len->second.empty())
	{
		this->_error = 400;
		return ;
	}

	size_t expectedLength = std::strtoul(len->second.c_str(), NULL, 10);
	size_t actualLength = this->_body.size();

	if (actualLength != expectedLength)
	{
		this->_error = 400;
		return ;
	}
}

std::string HttpRequest::toString()
{
	std::string request;

	request += this->_method + " " + this->_path + " " + this->_version + "\r\n";
	std::map<std::string, std::string>::const_iterator it = this->_headers.begin();
	while (it != this->_headers.end()) {
		request += it->first + ": " + it->second + "\r\n";
		++it;
	}
	request += "\r\n";
	request += this->_body;
	return (request);
}

bool HttpRequest::isValid(int *code) const
{
	if (this->_error && code)
		*code = this->_error;
	return (!this->_error);
}

//getter
const std::string &HttpRequest::getMethod() const {return (this->_method);}
const std::string &HttpRequest::getPath() const {return (this->_path);}
bool HttpRequest::empty() const {return (this->_is_empty);}

HttpRequest::~HttpRequest(){};

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

