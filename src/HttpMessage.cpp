#include "HttpMessage.hpp"

HttpMessage::HttpMessage()
{
}

HttpMessage::~HttpMessage()
{
}

const std::map<std::string, std::string>	&HttpMessage::getHeaders() const {return (this->_headers);}
const std::string &HttpMessage::getHeader(std::string key) const {return (this->_headers.at(key));}
const std::string	&HttpMessage::getBody() const {return (this->_body);}
const std::string	&HttpMessage::getVersion() const {return (this->_version);}