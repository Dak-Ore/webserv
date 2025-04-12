#ifndef HTTPPARSER_HPP
# define HTTPPARSER_HPP

# include <iostream>
# include <map>
# include <cstring>
# include <exception>
# include <sstream>
# include <cstdlib>
# include <fstream>

#define MAX_HEADERS 100

class HTTPParser
{
public:
    // Default Constructor
    HTTPParser(std::string request);
    // Destructor
    ~HTTPParser();    
	void parseRequestLine(std::istringstream& stream);
	void parseHeaders(std::istringstream& stream);
	void parseBody(std::istringstream& stream);
	void validateBodySize();
	void	checkHeader();
	void	checkAllowed(std::string allowedHeaders[], size_t allowedCount);
	std::string							method;
	std::string							path;
	std::string							version;
	std::map<std::string, std::string>	headers;
	std::string							body;
};

#endif
