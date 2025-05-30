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
	this->_type = ref._type;
	return (*this);
}

EPollEvent::EPollEvent(epoll_event ev) :
	_event(ev)
{
	if (ev.events & EPOLLIN)
		this->_type = IN;
	else if (ev.events & EPOLLOUT)
		this->_type = OUT;
	else
		this->_type = UNKNOWN;
}

EPollEvent::~EPollEvent()
{
}

int EPollEvent::getFd() const {
	return (this->_event.data.fd);
}

const epoll_event* EPollEvent::raw() const {
	return (&this->_event);
}

EPollEvent::type EPollEvent::getType() const
{
	return (this->_type);
}