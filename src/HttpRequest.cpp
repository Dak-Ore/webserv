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
	// if (!allowed.empty())
	// {
	// 	bool	find = 0;
	// 	for (std::vector<std::string>::iterator it = allowed.begin(); it != allowed.end(); it++)
	// 		if ((find = this->_method == *it) == 1)
	// 			break;
	// 	if (!find)
	// 	{
	// 		this->_error = 405;
	// 		return ;
	// 	}
	// }
	parseHeaders(stream);

	if (this->_headers.size() > MAX_HEADERS)
	{
		this->_error = 431;
		return ;
	}
	parseBody(stream);
	std::cout << "oui" << std::endl;
	std::cout << this->toString() << std::endl;
	if (_method == "POST")
		validateBodySize();
	if (!this->_headers["Cookie"].empty())
		parseCookie();
	hasMultipart();
}

void HttpRequest::parseCookie()
{
	std::string	content = this->_headers["Cookie"];
	std::string	key;
	std::string	value;
	bool		pos = 0;
	for (size_t	i = 0; i < content.size(); i++)
	{
		if (content[i] == ' ')
			continue;
		if (content[i] == '=')
		{
			pos = 1;
			continue;
		}
		if (content[i] == ';')
		{
			this->_cookies.insert(std::pair<std::string, std::string>(key,value));
			key.clear();
			value.clear();
			pos = 0;
			continue;
		}
		if (pos == 0)
			key += content[i];
		else
			value += content[i];
	}
	this->_cookies.insert(std::pair<std::string, std::string>(key,value));
	this->_headers.erase("Cookie");
}

void HttpRequest::parseOpt()
{
	size_t	pos_start = this->_path.find("?");
	std::string content = this->_path.substr(pos_start, this->_path.size() - pos_start);
	this->_path = this->_path.substr(0, pos_start);
	std::string	key;
	std::string	value;
	bool		pos = 0;
	for (size_t	i = 0; i < content.size(); i++)
	{
		if (content[i] == '=')
		{
			pos = 1;
			continue;
		}
		if (content[i] == '&')
		{
			this->_opt.insert(std::pair<std::string, std::string>(key,value));
			key.clear();
			value.clear();
			pos = 0;
			continue;
		}
		if (pos == 0)
			key += content[i];
		else
			value += content[i];
	}
	this->_opt.insert(std::pair<std::string, std::string>(key,value));
	for (std::map<std::string, std::string>::iterator it = _opt.begin(); it != _opt.end() ; it++)
		std::cout << it->first << " : " << it->second << std::endl;
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

	if (this->_path.find("?") != std::string::npos)
		parseOpt();

	if (this->_version != "HTTP/1.1")
		this->_error = 500;
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
	std::map<std::string, std::string>::iterator len = this->_headers.find(CONTENT_LENGHT);
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

std::string HttpRequest::toString() const
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

bool HttpRequest::isValid() const
{
	return (!this->_error);
}

int HttpRequest::getErrorCode() const
{
	return (this->_error);
}

void HttpRequest::hasMultipart()
{
	std::map<std::string, std::string>::iterator it = _headers.find("Content-Type");
	if (it == _headers.end())
		return;

	const std::string& contentType = it->second;
	if (contentType.find("multipart/form-data") != 0)
		return;

	std::string boundaryKey = "boundary=";
	size_t boundaryPos = contentType.find(boundaryKey);
	if (boundaryPos == std::string::npos)
	{
		_error = 400;
		return;
	}

	std::string boundary = "--" + contentType.substr(boundaryPos + boundaryKey.length());
	parseMultipartBody(boundary);
}

void HttpRequest::parseMultipartBody(std::string boundary)
{
	size_t pos = 0;
	std::cout << "oui" << std::endl;
	std::string upload = "upload"; // TO CHANGE
	while ((pos = this->_body.find(boundary, pos)) != std::string::npos)
	{
		pos += boundary.length();
		if (this->_body.compare(pos, 2, "\r\n") == 0)
			pos += 2;

		size_t nextPart = this->_body.find(boundary, pos);
		if (nextPart == std::string::npos)
			break;

		std::string part = this->_body.substr(pos, nextPart - pos);
		size_t headerEnd = part.find("\r\n\r\n");
		if (headerEnd == std::string::npos)
			continue;

		std::string headerSection = part.substr(0, headerEnd);
		std::string content = part.substr(headerEnd + 4);
		if (content.size() >= 2 && content.substr(content.size() - 2) == "\r\n")
			content = content.substr(0, content.size() - 2);

		std::istringstream headerStream(headerSection);
		std::string line;
		std::string name, filename;
		while (std::getline(headerStream, line))
		{
			int	eof = line.size() - 1;
			if (line[eof] == '\r') line.erase(eof);
			if (line.find("Content-Disposition:") == 0) {
				name = utils::getHeaderParam(line, "name");
				filename = utils::getHeaderParam(line, "filename");
			}
		}
	
		if (!filename.empty()) {
			saveFile(filename, content, upload); 
		}
	}
}

void HttpRequest::saveFile(const std::string& filename, const std::string& content, std::string &path)
{
	handleUploadDir(path);
	std::string	upload = utils::joinPath(path, filename);
	std::ofstream fichier;
	fichier.open(upload.c_str(), std::ofstream::out);
	if (!fichier.is_open())
		throw std::runtime_error("Failed to create file: " + filename);
	fichier << content;
	fichier.close();
}

void HttpRequest::handleUploadDir(const std::string& path)
{
	if (!utils::isDirectory(path))
		if (mkdir(path.c_str(), 0755) != 0)
			throw std::runtime_error("Failed to create upload directory");
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
// 			break;cd
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

