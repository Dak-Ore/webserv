#pragma once

#include "HttpMessage.hpp"

#include <string>

class HttpResponse : HttpMessage
{
private:
	int _status_code;
public:
	HttpResponse();
	HttpResponse(int status_code);
	~HttpResponse();
	static std::string getReason(int code);
	std::string getReason();
	std::string toString();
	void setBody(std::string body);
};
