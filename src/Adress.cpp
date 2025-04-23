#include "Adress.hpp"

#include <sstream>
#include <stdexcept>
#include <sys/socket.h>
#include <netdb.h>

std::string Adress::hostToString(int host)
{
	unsigned char bytes[4];
	bytes[0] = host & 0xFF;
	bytes[1] = (host >> 8) & 0xFF;
	bytes[2] = (host >> 16) & 0xFF;
	bytes[3] = (host >> 24) & 0xFF;

	std::ostringstream oss;
	oss << (int)bytes[0] << '.' << (int)bytes[1] << '.' << (int)bytes[2] << '.' << (int)bytes[3];
	return oss.str();
}

Adress::Adress()
{
}

Adress::Adress(int fd)
{
	std::pair<int, int> adress;;

	struct sockaddr_in server_addr;
	socklen_t addrlen = sizeof(server_addr);
	if (getsockname(fd, (struct sockaddr*)&server_addr, &addrlen) == -1)
		throw std::runtime_error("Invalid fd");
	this->_host = server_addr.sin_addr.s_addr;
	this->_port = ntohs(server_addr.sin_port);
}

Adress::Adress(int host, int port)
{
	this->_host = host;
	this->_port = port;
	this->_host_string = Adress::hostToString(this->_host);
}

Adress::Adress(const Adress &ref)
{
	*this = ref;
}

const Adress &Adress::operator=(const Adress &ref)
{
	this->_host = ref._host;
	this->_port = ref._port;
	this->_host_string = ref._host_string;
	return (*this);
}

Adress::~Adress()
{
}

int Adress::host() const {return (this->_host);}
const std::string &Adress::host_str() const {return (this->_host_string);}
int Adress::port() const {return (this->_port);}