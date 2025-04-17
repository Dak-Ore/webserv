#include "HttpResponse.hpp"

#include <sstream>

std::string intToString(int value) {
	std::ostringstream oss;
	oss << value;
	return oss.str();
}

HttpResponse::HttpResponse() : HttpMessage()
{
	this->_version = "HTTP/1.1";
	this->_status_code = 200;
}

HttpResponse::HttpResponse(int status_code) : HttpMessage()
{
	this->_version = "HTTP/1.1";
	this->_status_code = status_code;
}

HttpResponse::~HttpResponse()
{
}


std::string HttpResponse::toString()
{
	std::string request;

	request += this->_version + " " + intToString(this->_status_code) + " " + this->getReason() + "\r\n";
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
	
	default: reason = "REASON"; break;
	}
	return (reason);
}

std::string HttpResponse::getReason()
{
	return HttpResponse::getReason(this->_status_code);
}

void HttpResponse::setBody(std::string body){
	this->_body = body;
}