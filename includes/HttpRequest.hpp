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
# include <vector>

#define MAX_HEADERS 100

class HttpRequest : public HttpMessage
{
private:
	bool			_is_empty;
	std::string		_method;
	std::string		_path;
	int				_error;
	std::string		_rawOpt;
	void parseRequestLine(std::istringstream& stream);
	void parseHeaders(std::istringstream& stream);
	void parseBody(std::istringstream& stream);
	void validateBodySize();
	void parseOpt();
	void parseCookie();
public:
	HttpRequest();
    HttpRequest(const std::string &request);
    ~HttpRequest();
	const std::string &getMethod() const;
	const std::string &getPath() const;
	const std::string &getRawOpt() const;
	std::string toString() const;
	bool		empty() const;
	bool		isValid() const;
	int			getErrorCode() const;
};

#endif
