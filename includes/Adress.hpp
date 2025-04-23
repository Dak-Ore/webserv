#pragma once

#include <string>

class Adress
{
private:
	int _host;
	std::string _host_string;
	int _port;
public:
	static std::string hostToString(int host);
	Adress();
	Adress(int fd);
	Adress(int host, int port);
	Adress(const Adress &ref);
	const Adress &operator=(const Adress &ref);
	~Adress();

	int host() const;
	const std::string &host_str() const;
	int port() const;
};