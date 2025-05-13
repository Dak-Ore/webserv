#include "HttpMessage.hpp"

const std::map<std::string, std::string>	&HttpMessage::getHeaders() const {return (this->_headers);}
std::string HttpMessage::getHeader(std::string key) const
{
	if (this->_headers.find(key) != this->_headers.end())
		return this->_headers.at(key);
	else
		return ("");
}
const std::map<std::string, std::string>	&HttpMessage::getCookies() const {return (this->_cookies);}
std::string HttpMessage::getCookie(std::string key) const
{
	if (this->_cookies.find(key) != this->_cookies.end())
		return this->_cookies.at(key);
	else
		return ("");
}
const std::string	&HttpMessage::getBody() const {return (this->_body);}
const std::string	&HttpMessage::getVersion() const {return (this->_version);}