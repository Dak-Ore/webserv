#include "HttpResponse.hpp"
#include "HttpClient.hpp"
#include "utils.hpp"

#include <stdexcept>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <set>
#include <map>
#include <exception>

HttpResponse::HttpResponse(int status_code) : HttpMessage(),
	_status_code(status_code),
	_bodyFd(-1),
	_client(NULL)
{
	this->_version = "HTTP/1.1";
}

HttpResponse::~HttpResponse()
{
	if (this->_bodyFd != -1)
		::close(this->_bodyFd);
}

std::string HttpResponse::toString()
{
	if (this->_bodyFd == -1)
		this->_setHeader(CONTENT_LENGHT, utils::numToString(this->_body.size()));
	this->_setHeader("Connection", "close");

	std::string request;

	request += this->_version + " " + utils::numToString(this->_status_code) + " " + this->getReason() + "\r\n";
	std::map<std::string, std::string>::const_iterator it = this->_headers.begin();
	while (it != this->_headers.end())
	{
		request += it->first + ": " + it->second + "\r\n";
		++it;
	}
	request += this->_getCookieHeader();
	request += "\r\n";
	if (this->_bodyFd == -1)
		request += this->_body;
	return (request);
}

std::string HttpResponse::_getCookieHeader()
{
	std::string header;
	std::map<std::string, std::string>::iterator it;
    for (it = this->_cookies.begin(); it != this->_cookies.end(); ++it)
		header += SET_COOKIE + std::string(": ") + it->first + "=" + it->second + "\r\n";
	return (header);
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

void HttpResponse::closeBody()
{
	if (this->_bodyFd != -1)
	{
		::close(this->_bodyFd);
		this->_bodyFd = -1;
	}
}

void HttpResponse::setBody(const std::string &body)
{
	this->closeBody();
	this->_body = body;
}

bool HttpResponse::setBodySource(const std::string &file_name)
{
	if (!utils::fileExists(file_name))
	{
		this->_status_code = 404;
		return (false);
	}
	this->closeBody();
	this->_bodyFd = open(file_name.c_str(), O_RDONLY);
	if (this->_bodyFd == -1)
	{
		this->_status_code = 403;
		return (false);
	}
	this->_setContentType(file_name);
	this->_setHeader(CONTENT_LENGHT, utils::numToString((size_t)utils::getFileSize(file_name)));
	return (true);
}

void HttpResponse::setCode(int code)
{
	this->_status_code = code;
}

void HttpResponse::setHeader(const std::string &key, const std::string &value)
{
	static const std::string specials[] = { CONTENT_LENGHT, CONTENT_TYPE, SET_COOKIE };
	static const std::set<std::string> specialKeys(
		specials, specials + sizeof(specials)/sizeof(specials[0])
	);

	if (specialKeys.count(key))
		throw std::runtime_error("Invalid set Header");
	return (this->_setHeader(key, value));
}

void HttpResponse::_setHeader(const std::string &key, const std::string &value)
{
	this->_headers[key] = value;
}

void HttpResponse::setCookie(const std::string &key, const std::string &value, std::map<cookie_options, std::string> options)
{
	std::string &cookie = this->_cookies[key];
	cookie = value;
	for (size_t i = 0; i < options.size(); i++)
	{
		break ;
		cookie += "; ";
	}
}

void HttpResponse::_setContentType(const std::string& file_name)
{
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
	this->_setHeader(CONTENT_TYPE, (it != mimeTypes.end()) ? it->second : DEFAULT_CONTENT_TYPE);
}

void HttpResponse::bindClient(const HttpClient &client)
{
	this->_client = &client;
}

void HttpResponse::send()
{
	if (this->_client == NULL)
		throw std::runtime_error("Client is NULL");
	return (this->send(this->_client->getFd()));
}

void HttpResponse::send(int fd)
{
	std::string str = this->toString();
	bool readBody = (this->_bodyFd != -1);

	int flags = (readBody) ? MSG_MORE : 0;
	if (::send(fd, str.c_str(), str.size(), MSG_NOSIGNAL | flags) < 0)
		return ;
	if (readBody)
	{
		char buffer[1024];
		ssize_t bytes;
		while (true)
		{
			bytes = read(this->_bodyFd, buffer, sizeof(buffer));
			if (bytes == 0)
				break ;
			else if (bytes == -1)
				break ;
			if (::send(fd, buffer, bytes, MSG_NOSIGNAL) < 0)
				break ;
		}
		this->closeBody();
	}
}