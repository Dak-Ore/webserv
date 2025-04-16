#pragma once

#include "EPollEvent.hpp"

#define EPOLL_MAX_EVENTS 10

class EPoll
{
private:
	int _fd;
	EPollEvent _events[EPOLL_MAX_EVENTS];
	static int setNonBlocking(int fd);
	void wait();
	void add(int fd, int flags);
public:
	EPoll();
	~EPoll();

	EPollEvent *getEvents();

	void addSocket(int fd);
	void addClient(int fd);
	void remove(int fd, bool close_fd = true);
};

