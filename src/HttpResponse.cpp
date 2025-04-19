#include "HttpResponse.hpp"
#include "utils.hpp"

#include <sstream>
#include <fstream>
#include <stdexcept>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>

HttpResponse::HttpResponse(int status_code) : HttpMessage(),
	_status_code(status_code)
{
	this->_version = "HTTP/1.1";
}

HttpResponse::~HttpResponse()
{
}

std::string HttpResponse::toString()
{
	this->setHeader("Content-Length", utils::intToString(this->_body.size()));
	this->setHeader("Connection", "close");
	std::string request;

	request += this->_version + " " + utils::intToString(this->_status_code) + " " + this->getReason() + "\r\n";
	std::map<std::string, std::string>::const_iterator it = this->_headers.begin();
	while (it != this->_headers.end()) {
		request += it->first + ": " + it->second + "\r\n";
		++it;
	}
	request += "\r\n";
	request += this->_body;
	return (request);
}


std::string HttpResponse::getReason(int code)
{
	std::string reason;
	switch (code)
	{
	case 200: reason = "OK"; break;
	case 201: reason = "Created"; break;
	case 202: reason = "Accepted";	 break;
	case 203: reason = "Non-Authoritative Information"; break;
	case 204: reason = "No Content"; break;
	case 205: reason = "Reset Content"; break;
	case 206: reason = "Partial Content"; break;
	case 301: reason = "Moved Permanently"; break;
	case 302: reason = "Found"; break;
	case 303: reason = "See Other"; break;
	case 304: reason = "Not Modified"; break;
	case 305: reason = "Use Proxy"; break;
	case 307: reason = "Temporary Redirect"; break;
	case 400: reason = "Bad Request"; break;
	case 401: reason = "Unauthorized"; break;
	case 402: reason = "Bad Request"; break;
	case 403: reason = "Forbidden"; break;
	case 404: reason = "Not Found"; break;
	case 405: reason = "Method Not Allowed"; break;
	case 406: reason = "Not Acceptable"; break;
	case 407: reason = "Proxy Authentication Required"; break;
	case 408: reason = "Request Timeout"; break;
	case 409: reason = "Conflict"; break;
	case 410: reason = "Gone"; break;
	case 411: reason = "Length Required"; break;
	case 412: reason = "Precondition Failed"; break;
	case 413: reason = "Payload Too Large"; break;
	case 414: reason = "URI Too Long"; break;
	case 415: reason = "Unsupported Media Type"; break;
	case 416: reason = "Range Not Satisfiable"; break;
	case 417: reason = "Expectation Failed"; break;
	case 426: reason = "Upgrade Required"; break;
	case 500: reason = "Internal Server Error"; break;
	case 501: reason = "Not Implemented"; break;
	case 502: reason = "Bad Gateway"; break;
	case 503: reason = "Service Unavailable"; break;
	case 504: reason = "Gateway Timeout"; break;
	case 505: reason = "HTTP Version Not Supported"; break;
	default: reason = "REASON"; break;
	}
	return (reason);
}

std::string HttpResponse::getReason() const
{
	return HttpResponse::getReason(this->_status_code);
}

int HttpResponse::getCode() const {return (this->_status_code);}

void HttpResponse::setBody(const std::string &body)
{
	this->_body = body;
}

void HttpResponse::setBodySource(const std::string &file_name)
{
	if (!utils::fileExists(file_name))
	{
		this->_status_code = 404;
		return ;
	}	
	std::ifstream file(file_name.c_str());
	if (!file)
	{
		this->_status_code = 403;
		return ;
	}
	this->setContentType(file_name);
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	this->_body = buffer.str();
}

void HttpResponse::setCode(int code)
{
	this->_status_code = code;
}

void HttpResponse::setHeader(const std::string &key, const std::string &value)
{
	this->_headers[key] = value;
}

void HttpResponse::setContentType(const std::string& file_name) {
	static std::map<std::string, std::string> mimeTypes;
	mimeTypes[".html"] = "text/html";
	mimeTypes[".css"] = "text/css";
	mimeTypes[".js"] = "application/javascript";
	mimeTypes[".json"] = "application/json";
	mimeTypes[".jpg"] = "image/jpeg";
	mimeTypes[".jpeg"] = "image/jpeg";
	mimeTypes[".png"] = "image/png";
	mimeTypes[".txt"] = "text/plain";
	mimeTypes[".pdf"] = "application/pdf";

	std::string ext = file_name.substr(file_name.find_last_of('.'));
	std::map<std::string, std::string>::const_iterator it = mimeTypes.find(ext);
	if (it != mimeTypes.end())
		this->setHeader("Content-Type", it->second);
	else
		this->setHeader("Content-Type", "application/octet-stream");
}

void HttpResponse::send(int fd)
{
	std::string str = this->toString();
	::send(fd, str.c_str(), str.size(), MSG_NOSIGNAL);
}