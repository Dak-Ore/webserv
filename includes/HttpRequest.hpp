#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

#include "HttpMessage.hpp"
#include "utils.hpp"
# include <iostream>
# include <map>
# include <cstring>
# include <exception>
# include <sstream>
# include <sys/stat.h>
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
	void parseRequestLine(std::istringstream& stream);
	void parseHeaders(std::istringstream& stream);
	void parseBody(std::istringstream& stream);
	void parseMultipartBody(std::string boundary);
	void validateBodySize();
	void parseOpt();
	void parseCookie();
	void hasMultipart();
	void handleUploadDir(const std::string& path);
	void saveFile(const std::string& filename, const std::string& content, std::string &path);
public:
	HttpRequest();
    HttpRequest(const std::string &request);
    ~HttpRequest();
	const std::string &getMethod() const;
	const std::string &getPath() const ;
	std::string toString() const;
	bool		empty() const;
	bool		isValid() const;
	int			getErrorCode() const;
};

#endif
