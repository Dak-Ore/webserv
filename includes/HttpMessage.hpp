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
	std::map<std::string, std::string>	getHeaders() const;
	std::string							getBody() const;
	std::string							getVersion() const;
	virtual std::string					toString() = 0;
};

