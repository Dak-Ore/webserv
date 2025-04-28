#pragma once

#include "Adress.hpp"

#include <sys/socket.h>
#include <string>

class Socket
{
private:
	void listen();
protected:
	int _fd;
	Adress _adress;

public:
	Socket();
	Socket(std::string hostname, std::string service);
	~Socket();
	int getFd() const;
	const Adress &getAdress() const;

	void close();
};
