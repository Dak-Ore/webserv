#pragma once

#include <string>
#include <ostream>

class Adress
{
private:
	int _host;
	int _port;
	struct addrinfo *_addrinfo;
public:
	static std::string hostToString(int host);
	static int createSocket(const Adress &adress);

	Adress();
	Adress(int fd);
	Adress(int host, int port);
	Adress(std::string host, std::string port);
	Adress(const Adress &ref);
	const Adress &operator=(const Adress &ref);
	bool operator==(const Adress &ref);
	~Adress();

	int host() const;
	std::string host_str() const;
	int port() const;
	std::string port_str() const;
	std::string str() const;

	int createSocket();
	bool bind(int fd);
};

std::ostream& operator<<(std::ostream& os, const Adress &ref);