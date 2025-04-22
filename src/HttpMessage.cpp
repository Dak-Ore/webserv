#include "HttpMessage.hpp"

const std::map<std::string, std::string>	&HttpMessage::getHeaders() const {return (this->_headers);}
const std::string &HttpMessage::getHeader(std::string key) const {return (this->_headers.at(key));}
const std::map<std::string, std::string>	&HttpMessage::getCookies() const {return (this->_cookies);}
const std::string &HttpMessage::getCookie(std::string key) const {return (this->_cookies.at(key));}
const std::string	&HttpMessage::getBody() const {return (this->_body);}
const std::string	&HttpMessage::getVersion() const {return (this->_version);}