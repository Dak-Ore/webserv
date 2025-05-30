#include "HttpRequest.hpp"
#include "HttpClient.hpp"
#include "Config.hpp"
#include "Webserv.hpp"
#include <algorithm>
#include <ctime>

#define REQUEST_MAX_SIZE 8192

HttpRequest::HttpRequest() : HttpMessage(),
	_createdAt(std::time(NULL)), _is_empty(true), _header_ready(false), _ready(false), _config(NULL), _error(0)
 {
 }
 

// this->_error = 400;
//     if (this->_headers.size() > MAX_HEADERS)
//         this->_error = 431;

//     if (!readBody())
// 	{
//         this->_error = 413; // Payload Too Large
//         return;
//     }
//     if (_method == "POST")
//         validateBodySize();

//     if (!this->_headers["Cookie"].empty())
//         parseCookie();
// }

bool HttpRequest::read(const std::string &content)
{
	if (!this->_header_ready)
		this->readHeaders(content);
	else
		this->readBody(content);
	return (this->_ready);
}

void HttpRequest::readHeaders(const std::string &content)
{
	size_t header_limit;
	std::string body;

	// ( > REQUEST_MAX_SIZE)
	// this->_error = 431;
	this->_buffer += content;
	if (this->_buffer.size() > 0)
		this->_is_empty = false;
	header_limit = this->_buffer.find("\r\n\r\n");
	if (header_limit != std::string::npos)
	{
		body = this->_buffer.substr(header_limit + 4);
		this->_buffer.substr(0, header_limit);
		this->parseHeaders(this->_buffer);
		if (this->_error)
			throw std::exception();
		if (this->_buffer.size() > 0)
			this->_is_empty = false;
		this->_buffer.clear();
		this->_header_ready = true;
	}
	if (this->_header_ready)
		this->readBody(body);

}

void HttpRequest::readBody(const std::string &content)
{
	unsigned int expectedSize = utils::stringToInt(this->getHeader(CONTENT_LENGHT));
	if (this->_config)
	{
		if (expectedSize > this->_config->getClientMaxBodySize())
			throw std::exception();
	}
	this->_body += content;
	if (expectedSize == this->_body.size())
		this->_ready = true;
	else if (expectedSize < this->_body.size())
		throw std::exception();
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
	this->_rawCookie = content;
	this->_headers.erase("Cookie");
}

void HttpRequest::parseOpt()
{
	size_t	pos_start = this->_path.find("?");
	this->_rawOpt = this->_path.substr(pos_start + 1);
	this->_path = this->_path.substr(0, pos_start);
	std::string	key;
	std::string	value;
	bool		pos = 0;
	for (size_t	i = 0; i < this->_rawOpt.size(); i++)
	{
		if (this->_rawOpt[i] == '=')
		{
			pos = 1;
			continue;
		}
		if (this->_rawOpt[i] == '&')
		{
			this->_opt.insert(std::pair<std::string, std::string>(key,value));
			key.clear();
			value.clear();
			pos = 0;
			continue;
		}
		if (pos == 0)
			key += this->_rawOpt[i];
		else
			value += this->_rawOpt[i];
	}
	this->_opt.insert(std::pair<std::string, std::string>(key,value));
	for (std::map<std::string, std::string>::iterator it = _opt.begin(); it != _opt.end() ; it++)
		std::cout << it->first << " : " << it->second << std::endl;
	std::cout << this->_rawOpt << std::endl;
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
		this->_error = 406;
		return ;
	}

	if (this->_path.find("?") != std::string::npos)
		parseOpt();

	if (this->_version != "HTTP/1.1")
		this->_error = 500;
}


void HttpRequest::parseHeaders(const std::string &headers)
{
	std::istringstream stream(headers);
	std::string line;

    this->parseRequestLine(stream);
	while (std::getline(stream, line) && line != "\r") {
		if (line.empty() || line == "\r")
			break;
		size_t pos = line.find("\r");
		if (pos != std::string::npos)
			line = line.substr(0, pos);
		pos = line.find(": ");
		if (pos != std::string::npos) {
			std::string key = line.substr(0, pos);
			if (this->_headers.find(key) != this->_headers.end())
			{
				this->_error = 407; // DUPLICATE HEADER
				return ;
			}
			this->_headers[key] = line.substr(pos + 2);
		}
	}
	if (!this->_headers["Cookie"].empty())
		parseCookie();

	if (this->_headers.find("Host") == this->_headers.end() || this->_headers["Host"].empty() )
	{
		this->_error = 405;
		return;
	}
}

void HttpRequest::parseBody(std::istringstream& stream)
{
	std::string line;
	while (std::getline(stream, line))
	{
		this->_body += line + "\n";
	}
}

void HttpRequest::validateBodySize()
{
	std::map<std::string, std::string>::iterator len = this->_headers.find(CONTENT_LENGHT);
	if (len == this->_headers.end() || len->second.empty())
	{
		this->_error = 404;
		return ;
	}

	size_t expectedLength = std::strtoul(len->second.c_str(), NULL, 10);
	size_t actualLength = this->_body.size();

	if (actualLength != expectedLength)
	{
		this->_error = 403;
		return ;
	}
}

void HttpRequest::checkAllowedMethods()
{
	const std::vector<std::string> &allowed = this->_config->getAllowedMethods();
    if (!allowed.empty()) 
	{
		if (std::find(allowed.begin(), allowed.end(), this->_method) == allowed.end())
		{
            this->_error = 405;
            return;	
		}
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
	std::map<std::string, std::string>::iterator it = this->_headers.find("Content-Type");
	if (it == _headers.end())
		return;

	const std::string& contentType = it->second;
	if (contentType.find("multipart/form-data") != 0)
		return;
	else if (!this->_config->getUploadEnabled())
	{
		this->_error = 403;
		return;
	}

	std::string boundaryKey = "boundary=";
	size_t boundaryPos = contentType.find(boundaryKey);
	if (boundaryPos == std::string::npos)
	{
		_error = 402;
		return;
	}

	std::string boundary = "--" + contentType.substr(boundaryPos + boundaryKey.length());
	parseMultipartBody(boundary);
}

void HttpRequest::parseMultipartBody(std::string boundary)
{
	size_t pos = 0;
	size_t start = this->_body.find("Content-Disposition:");
	if (start == std::string::npos)
		return;
	size_t end = this->_body.find("\r\n", pos);
	if (end == std::string::npos)
		end = this->_body.length();
	std::string contentDispositionLine = this->_body.substr(start, end - start);
	std::string filename = utils::getHeaderParam(contentDispositionLine, "filename");
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
			saveFile(filename, content); 
		}
	}
}

void HttpRequest::saveFile(const std::string& filename, const std::string& content)
{
	std::string path = handleUploadDir();
	std::cout << path << std::endl;
	std::string	upload = utils::joinPath(path, filename);
	std::ofstream fichier;
	fichier.open(upload.c_str(), std::ofstream::out);
	if (!fichier.is_open())
		throw std::runtime_error("Failed to create file: " + filename);
	fichier << content;
	fichier.close();
}

std::string HttpRequest::handleUploadDir()
{
	std::string	path;
	if (!this->_config->getUploadPath().empty())
	{
		std::vector<std::string>::const_iterator it = this->_config->getUploadPath().begin();
		while (it != this->_config->getUploadPath().end() && utils::isDirectory(*it))
			it++;
		if (it != this->_config->getUploadPath().end())
			return *it;
	}	
	path = "uploads";
	if (!utils::isDirectory(path))
		if (mkdir(path.c_str(), 0755) != 0)
			throw std::runtime_error("Failed to create uploads directory");
	return (path);
}

//getter
const std::string &HttpRequest::getMethod() const {return (this->_method);}
const std::string &HttpRequest::getPath() const {return (this->_path);}
const std::string &HttpRequest::getBody() const {return (this->_body);}
Config *HttpRequest::getConfig() const {return (this->_config);}
void HttpRequest::setConfig(Config *config)
{
	this->_config = config;
	this->checkAllowedMethods();
}
const std::string &HttpRequest::getRawOpt() const {return (this->_rawOpt);}
const std::string &HttpRequest::getRawCookie() const {return (this->_rawCookie);}
bool HttpRequest::empty() const {return (this->_is_empty);}

HttpRequest::~HttpRequest(){};

bool HttpRequest::isHeaderReady() const
{
	return (this->_header_ready);
}

bool HttpRequest::isReady() const
{
	return (this->_ready);
}

time_t 		HttpRequest::getCreatedAt() const
{
	return (this->_createdAt);
}