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
	std::string getReason() const;
	std::string toString();
	void setBody(const std::string &body);
	void setBodySource(const std::string &file_name);
	void setCode(int code);
	void setHeader(const std::string &key, const std::string &value);
	void setContentType(const std::string& file_name);
	int  getCode() const;
	void send(int fd);
};
