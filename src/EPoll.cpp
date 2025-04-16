#include "EPoll.hpp"

#include <stdexcept>
#include <sys/epoll.h>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>

EPoll::EPoll() :
	_fd(-1)
{
	this->_fd = epoll_create1(0);
}

EPoll::~EPoll()
{
	if (this->_fd >= 0)
		close(this->_fd);
}

int EPoll::setNonBlocking(int fd)
{
	int flags = fcntl(fd, F_GETFL, 0);
	return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

void EPoll::add(int fd, int flags)
{
	EPoll::setNonBlocking(fd);
	epoll_event ev;
	std::memset(&ev, 0, sizeof(ev));
	ev.events = flags;
	ev.data.fd = fd;
	if (epoll_ctl(this->_fd, EPOLL_CTL_ADD, fd, &ev))
		throw std::runtime_error("epoll_ctl failed");
}

void EPoll::addSocket(int fd)
{
	this->add(fd, EPOLLIN);
}

void EPoll::addClient(int fd)
{
	this->add(fd, EPOLLIN | EPOLLET);
}

void EPoll::remove(int fd, bool close_fd)
{
	if (close_fd)
		close(fd);
	epoll_ctl(this->_fd, EPOLL_CTL_DEL, fd, NULL);
}

void EPoll::wait()
{
	epoll_event raw_events[EPOLL_MAX_EVENTS];

	int n = epoll_wait(this->_fd, raw_events, EPOLL_MAX_EVENTS, -1);
	for (int i = 0; i < n; i++)
		this->_events[i] = EPollEvent(raw_events[i]);
}

EPollEvent *EPoll::getEvents()
{
	this->wait();
	return (this->_events);
}
