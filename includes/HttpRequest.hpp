#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

#include "HttpMessage.hpp"

# include <iostream>
# include <map>
# include <cstring>
# include <exception>
# include <sstream>
# include <sys/stat.h>
# include <cstdlib>
# include <fstream>
# include <vector>
#include <ctime>

#define MAX_HEADERS 100

class Webserv;
class Config;
class HttpClient;

class HttpRequest : public HttpMessage
{
private:
	time_t			_createdAt;
	bool			_is_empty;
	bool			_header_ready;
	bool			_ready;
	std::string		_buffer;
	Config			*_config;
	std::string		_method;
	std::string		_path;
	int				_error;
	std::string		_rawOpt;
	void readHeaders(const std::string &content);
	void readBody(const std::string &content);
	void parseRequestLine(std::istringstream& stream);
	void parseHeaders(const std::string &headers);
	void parseBody(std::istringstream& stream);
	void parseMultipartBody(std::string boundary);
	void validateBodySize();
	void parseOpt();
	void parseCookie();
	void checkAllowedMethods();
	void hasMultipart();
	std::string handleUploadDir();
	void saveFile(const std::string& filename, const std::string& content);
public:
    HttpRequest();
    ~HttpRequest();
	const std::string &getMethod() const;
	const std::string &getPath() const;
	const std::string &getRawOpt() const;
	Config *getConfig() const;
	void setConfig(Config *config);
	std::string toString() const;
	bool		empty() const;
	bool		isValid() const;
	bool		isHeaderReady() const;
	bool		isReady() const;
	int			getErrorCode() const;
	bool		read(const std::string &content);
	time_t 		getCreatedAt() const;
};

#endif
