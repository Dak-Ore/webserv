#pragma once

#include "Socket.hpp"

class Adress;

class ServerSocket : public Socket
{
private:
	void listen();
public:
	ServerSocket();
	ServerSocket(const Adress &adress);
};