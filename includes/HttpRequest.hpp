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
	std::string		_method;
	std::string		_path;
public:
	HttpRequest();
    HttpRequest(std::string request);
    ~HttpRequest();
	std::string	getMethod();
	std::string	getPath();
	std::string toString();
private:
	void parseRequestLine(std::istringstream& stream);
	void parseHeaders(std::istringstream& stream);
	void parseBody(std::istringstream& stream);
	void validateBodySize();
};

#endif
