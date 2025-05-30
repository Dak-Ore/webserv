#pragma once

#include "Adress.hpp"

#include <sys/socket.h>
#include <string>

class Socket
{
private:
	int _fd;
protected:
	void setFd(int fd);
	Adress _adress;

public:
	Socket();
	~Socket();
	int getFd() const;
	const Adress &getAdress() const;

	void close();
	std::string read();
	void send(std::string &response);

	class closedSocketException : std::exception
	{};
};
