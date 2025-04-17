#pragma once

#include "HttpMessage.hpp"

#include <string>

class HttpResponse : HttpMessage
{
private:
	int _status_code;
	// bool _keep_alive;
public:
	HttpResponse(int status_code = 200);
	~HttpResponse();
	static std::string getReason(int code);
	std::string getReason();
	std::string toString();
	void setBody(std::string body);
	void setBodySource(std::string file_name);
	void setCode(int code);
	void setHeader(std::string key, std::string value);
	void send(int fd);
};
