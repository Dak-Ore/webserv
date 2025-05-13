#include "Adress.hpp"
#include "Socket.hpp"

#include <sstream>
#include <stdexcept>
#include <exception>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

static sockaddr_in init_sock(int host, int port)
{
	sockaddr_in sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(port);
	sockaddr.sin_addr.s_addr = host;
	for (int i = 0; i < 8; ++i) {
		sockaddr.sin_zero[i] = 0;
	}
	return (sockaddr);
}

std::string Adress::hostToString(int host) {
	unsigned char bytes[4];
	bytes[0] = host & 0xFF;
	bytes[1] = (host >> 8) & 0xFF;
	bytes[2] = (host >> 16) & 0xFF;
	bytes[3] = (host >> 24) & 0xFF;

	std::ostringstream oss;
	oss << (int)bytes[0] << '.' << (int)bytes[1] << '.' << (int)bytes[2] << '.' << (int)bytes[3];
	return oss.str();
}

int Adress::createSocket() const
{
	int fd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (fd == -1)
		throw std::runtime_error("Failed to create socket");
	int opt = 1;
	if (::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		::close(fd);
		throw std::runtime_error("setsockopt SO_REUSEADDR failed");
	}
	return fd;
}

Adress::Adress() :
	_host(0), _port(0), _addrlen(sizeof(_sockaddr))
{
	this->_sockaddr = init_sock(0, 0);
}

Adress::Adress(Socket *socket)
{
	if (socket == NULL)
		throw std::runtime_error("Socket pointer is NULL");

	this->_addrlen = sizeof(_sockaddr);
	if (::getsockname(socket->getFd(), (struct sockaddr*)&this->_sockaddr, &this->_addrlen) == -1)
		throw std::runtime_error("Invalid fd");

	this->_host = _sockaddr.sin_addr.s_addr;
	this->_port = ntohs(_sockaddr.sin_port);
}

Adress::Adress(int host, int port) :
	_host(host), _port(port), _addrlen(sizeof(_sockaddr))
{
		this->_sockaddr = init_sock(host, port);
}

Adress::Adress(const std::string &host, const std::string &port)
{
	struct addrinfo hints = {};
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	struct addrinfo* res;
	int status = ::getaddrinfo(host.c_str(), port.c_str(), &hints, &res);
	if (status != 0)
		throw std::runtime_error(::gai_strerror(status));

	struct sockaddr_in* addr_in = (struct sockaddr_in*)res->ai_addr;
	this->_sockaddr = *addr_in;
	this->_addrlen = res->ai_addrlen;
	this->_host = addr_in->sin_addr.s_addr;
	this->_port = ntohs(addr_in->sin_port);
	::freeaddrinfo(res);
}

Adress::Adress(const sockaddr_in& sockaddr) :
	_sockaddr(sockaddr), _addrlen(sizeof(sockaddr_in))
{
	this->_host = sockaddr.sin_addr.s_addr;
	this->_port = ntohs(sockaddr.sin_port);
}

Adress::Adress(const Adress &ref)
{
	*this = ref;
}

const Adress &Adress::operator=(const Adress &ref)
{
	this->_host = ref._host;
	this->_port = ref._port;
	this->_sockaddr = ref._sockaddr;
	this->_addrlen = ref._addrlen;
	return (*this);
}

Adress::~Adress()
{
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

bool Adress::bind(int fd) const
{
	return (::bind(fd, (struct sockaddr*)&_sockaddr, _addrlen) == 0);
}

bool Adress::operator==(const Adress &ref) const
{
	return ((this->_host == 0 || ref._host == 0 || this->_host == ref._host) && this->_port == ref._port);
}


std::ostream& operator<<(std::ostream& os, const Adress& ref) {
	os << ref.str();
	return os;
}