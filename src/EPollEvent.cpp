#include "EPollEvent.hpp"

EPollEvent::EPollEvent()
{
	std::memset(&this->_event, 0, sizeof(this->_event));
}

EPollEvent::EPollEvent(EPollEvent const &ref)
{
	*this = ref;
}
EPollEvent &EPollEvent::operator=(const EPollEvent &ref)
{
	this->_event = ref._event;
	return (*this);
}

EPollEvent::EPollEvent(epoll_event ev) :
	_event(ev)
{
}

EPollEvent::~EPollEvent()
{
}

int EPollEvent::getFd() const {
	return (this->_event.data.fd);
}

void EPollEvent::setFd(int fd)
{
	this->_event.data.fd = fd;
}

epoll_event* EPollEvent::raw()
{
	return &this->_event;
}

const epoll_event* EPollEvent::raw() const {
	return &this->_event;
}