#pragma once

#include "HttpMessage.hpp"

#include <string>

enum cookie_options
{
	COOKIE_DOMAIN,
	COOKIE_EXPIRES,
	COOKIE_HTTPONLY, // no value
	COOKIE_MAX_AGE,
	COOKIE_PARTITIONNED,
	COOKIE_PATH,
	COOKIE_SECURE, // no value
	COOKIE_SAME_SITE
};

class HttpResponse : HttpMessage
{
private:
	int _status_code;
	int _bodyFd;
	// bool _keep_alive;
	void closeBody();
	void _setContentType(const std::string& file_name);
	void _setHeader(const std::string &key, const std::string &value);
	std::string _getCookieHeader();
public:
	HttpResponse(int status_code = 200);
	~HttpResponse();
	static std::string getReason(int code);
	std::string getReason() const;
	std::string toString();
	void setBody(const std::string &body);
	bool setBodySource(const std::string &file_name);
	void setCode(int code);
	void setHeader(const std::string &key, const std::string &value);
	void setCookie(const std::string &key, const std::string &value, std::map<cookie_options, std::string> options = std::map<cookie_options, std::string>());
	int  getCode() const;
	void send(int fd);
};
