#include "HttpMessage.hpp"

HttpMessage::HttpMessage()
{
}

HttpMessage::~HttpMessage()
{
}

std::map<std::string, std::string>	HttpMessage::getHeaders() const {return (this->_headers);}
std::string	HttpMessage::getBody() const {return (this->_body);}
std::string	HttpMessage::getVersion() const {return (this->_version);}