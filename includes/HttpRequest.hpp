#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

#include "HttpMessage.hpp"

# include <iostream>
# include <map>
# include <cstring>
# include <exception>
# include <sstream>
# include <cstdlib>
# include <fstream>

#define MAX_HEADERS 100

class HttpRequest : HttpMessage
{
private:
	bool			_is_empty;
	std::string		_method;
	std::string		_path;
	int				_error;
	void parseRequestLine(std::istringstream& stream);
	void parseHeaders(std::istringstream& stream);
	void parseBody(std::istringstream& stream);
	void validateBodySize();
public:
	HttpRequest();
    HttpRequest(const std::string &request);
    ~HttpRequest();
	const std::string &getMethod() const;
	const std::string &getPath() const ;
	std::string toString();
	bool		empty() const;
	bool		isValid(int *code = 0) const;
};

#endif
