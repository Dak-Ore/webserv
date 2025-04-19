#pragma once

#include <string>
#include <map>

class HttpMessage
{
protected:
	std::map<std::string, std::string>	_headers;
	std::string							_body;
	std::string							_version;
public:
	HttpMessage();
	~HttpMessage();
	const std::map<std::string, std::string>	&getHeaders() const;
	const std::string &getHeader(std::string key) const;
	const std::string							&getBody() const;
	const std::string							&getVersion() const;
	virtual std::string					toString() = 0;
};

