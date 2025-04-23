#pragma once

#include "Adress.hpp"

#include <sys/socket.h>
#include <string>
#include <unistd.h>

class Socket
{
private:
	void listen();
protected:
	bool _closeOnDestruct;
	int _fd;
	Adress _adress;

public:
	Socket();
	Socket(std::string hostname, std::string service);
	~Socket();

	int getFd() const;
	const Adress &getAdress() const;
};
