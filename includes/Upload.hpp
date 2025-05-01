#ifndef UPLOAD_HPP
# define UPLOAD_HPP

# include <iostream>
# include "ServerConfig.hpp"
# include "HttpRequest.hpp"
# include "HttpResponse.hpp"

class Upload
{
private:
	HttpRequest		_request;
	HttpResponse	_response;
	std::string		_boundary;
	std::string		_dir;
	ServerConfig	_config;
public:
    // Default Constructor
    Upload(HttpRequest &request, HttpResponse &response, std::string boundary, std::string dir, ServerConfig *ServerConfig);
    
    // Destructor
    ~Upload();
};

#endif
