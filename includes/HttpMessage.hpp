#pragma once

#include <string>
#include <map>

class HttpMessage
{
protected:
	std::map<std::string, std::string>	_headers;
	std::map<std::string, std::string>	_cookies;
	std::string							_body;
	std::string							_version;
public:
	const std::map<std::string, std::string>	&getHeaders() const;
	const std::string &getHeader(std::string key) const;
	const std::map<std::string, std::string>	&getCookies() const;
	const std::string &getCookie(std::string key) const;
	const std::string							&getBody() const;
	const std::string							&getVersion() const;
};

