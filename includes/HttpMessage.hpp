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
	std::map<std::string, std::string>	getHeaders();
	std::string							getBody();
	std::string							getVersion();
	virtual std::string					toString() = 0;
};

