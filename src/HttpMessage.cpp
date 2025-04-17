#include "HttpMessage.hpp"

HttpMessage::HttpMessage()
{
}

HttpMessage::~HttpMessage()
{
}

std::map<std::string, std::string>	HttpMessage::getHeaders(){return (this->_headers);}
std::string	HttpMessage::getBody(){return (this->_body);}
std::string	HttpMessage::getVersion(){return (this->_version);}