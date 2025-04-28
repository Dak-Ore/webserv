#include "Adress.hpp"

#include <sstream>
#include <stdexcept>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

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

int Adress::createSocket(const Adress &adress)
{
	int fd = ::socket(adress._addrinfo->ai_family, adress._addrinfo->ai_socktype, adress._addrinfo->ai_protocol);
	if (fd == -1)
		throw std::runtime_error("Failed to create socket");
	int i = 1;
	if (::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(i)) < 0)
	{
		::close(fd);
		throw std::runtime_error("setsockopt SO_REUSEADDR failed");
	}
	return (fd);
}

Adress::Adress() :
	_addrinfo(NULL)
{
}

Adress::Adress(int fd) :
	_addrinfo(NULL)
{
	std::pair<int, int> adress;

	struct sockaddr_in server_addr;
	socklen_t addrlen = sizeof(server_addr);
	if (getsockname(fd, (struct sockaddr*)&server_addr, &addrlen) == -1)
		throw std::runtime_error("Invalid fd");
	this->_host = server_addr.sin_addr.s_addr;
	this->_port = ntohs(server_addr.sin_port);
}

Adress::Adress(int host, int port) :
	_addrinfo(NULL)
{
	this->_host = host;
	this->_port = port;
}

Adress::Adress(std::string host, std::string port) :
	_addrinfo(NULL)
{
	struct addrinfo hints;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
	struct addrinfo *res;
	int status = ::getaddrinfo(host.c_str(), port.c_str(), &hints, &res);
	if (status != 0)
		throw std::runtime_error(::gai_strerror(status));

	struct sockaddr_in *addr_in = (struct sockaddr_in *)(res->ai_addr);
	this->_host = addr_in->sin_addr.s_addr;
	this->_port = res->ai_protocol;
	this->_addrinfo = res;
}

Adress::Adress(const Adress &ref)
{
	*this = ref;
}

const Adress &Adress::operator=(const Adress &ref)
{
	this->_host = ref._host;
	this->_port = ref._port;
	return (*this);
}

Adress::~Adress()
{
	if (this->_addrinfo)
		::freeaddrinfo(this->_addrinfo);
}

int Adress::host() const {return (this->_host);}
std::string Adress::host_str() const {return (Adress::hostToString(this->_host));}
int Adress::port() const {return (this->_port);}
std::string Adress::port_str() const
{
	std::stringstream s;
	s << this->_port;
	return (s.str());
}
std::string Adress::str() const
{
	std::stringstream s;
	s << Adress::hostToString(this->_host) << ":" << this->_port;
	return (s.str());
}

int Adress::createSocket()
{
	return (Adress::createSocket(*this));
}

bool Adress::bind(int fd)
{
	if (this->_addrinfo == NULL)
		throw new std::runtime_error("Can't bind empty adress");
	return (::bind(fd, this->_addrinfo->ai_addr, this->_addrinfo->ai_addrlen) == 0);
}