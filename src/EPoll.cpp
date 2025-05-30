#include "EPoll.hpp"
#include "Socket.hpp"
#include "HttpClient.hpp"
#include "CGI.hpp"
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
	for (std::set<int>::const_iterator it = this->_fds.begin(); it != this->_fds.end(); ++it)
		::close(*it);
	if (this->_fd >= 0)
		::close(this->_fd);
}

int EPoll::setNonBlocking(int fd)
{
	int flags = fcntl(fd, F_GETFL, 0);
	return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

void EPoll::add(int fd, int flags)
{
	epoll_event ev;
	std::memset(&ev, 0, sizeof(ev));
	ev.events = flags;
	ev.data.fd = fd;
	if (::epoll_ctl(this->_fd, EPOLL_CTL_ADD, fd, &ev))
   		throw std::runtime_error(std::string("epoll_ctl failed: ") + strerror(errno));
	this->_fds.insert(fd);
}

void EPoll::addSocket(const Socket &socket)
{
	int fd = socket.getFd();
	EPoll::setNonBlocking(fd);
	this->add(fd, EPOLLIN);
}

void EPoll::addClient(const HttpClient &client)
{
	int fd = client.getFd();
	EPoll::setNonBlocking(fd);
	this->add(fd, EPOLLIN);
}

void EPoll::addNewCgi(const CGI::Running &cgiProcess)
{
	int fd = cgiProcess.getFd();
	this->add(fd, EPOLLIN);
	this->_cgis[fd] = false;
}

void EPoll::setCgiReady(const CGI::Running &cgiProcess)
{
	int oldFd = cgiProcess.getFd();
	this->remove(oldFd, false);
	this->_cgis.erase(oldFd);

	int newFd = cgiProcess.getResponseBodyFd();
	this->add(newFd, EPOLLIN);
	this->_cgis[newFd] = true;
}

void EPoll::remove(int fd, bool close_fd)
{
	if (close_fd)
		::close(fd);
	::epoll_ctl(this->_fd, EPOLL_CTL_DEL, fd, NULL);
	this->_fds.erase(fd);
}

void EPoll::update(int fd, int flags)
{
    struct epoll_event event;
    event.events = flags; //| EPOLLONESHOT;
    event.data.fd = fd;
	::epoll_ctl(this->_fd, EPOLL_CTL_MOD, fd, &event);
}

void EPoll::wait()
{
	epoll_event raw_events[EPOLL_MAX_EVENTS];

	this->_events.clear();
	int n = ::epoll_wait(this->_fd, raw_events, EPOLL_MAX_EVENTS, 1);
	for (int i = 0; i < n; i++)
	{
		EPollEvent event(raw_events[i]);
		if (this->_cgis.find(raw_events->data.fd) != this->_cgis.end())
			event.setType(EPollEvent::CGI);
		this->_events.push_back(event);
	}
}

const std::vector<EPollEvent> &EPoll::getEvents()
{
	this->wait();
	return (this->_events);
}

void EPoll::setIn(const HttpClient &client)
{
	this->update(client.getFd(), EPOLLIN | EPOLLET);
}

void EPoll::setOut(const HttpClient &client)
{
	this->update(client.getFd(), EPOLLOUT);
}

void EPoll::remove(const Socket &socket, bool close_fd)
{
	this->remove(socket.getFd(), close_fd);
}

void EPoll::remove(const HttpClient &client, bool close_fd)
{
	this->remove(client.getFd(), close_fd);
}

void EPoll::remove(const CGI::Running &cgiProcess, bool close_fd)
{
	int fd = (cgiProcess.isHeadComplete()) ? cgiProcess.getResponseBodyFd() : cgiProcess.getFd();
	this->_cgis.erase(fd);
	this->remove(fd, close_fd);
}

