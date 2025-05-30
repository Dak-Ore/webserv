#pragma once

#include <sys/epoll.h>
#include <cstring>

class EPollEvent
{
public:
	enum type{
		UNKNOWN,
		IN,
		OUT,
		CGI
	};
private:
    epoll_event _event;
	type _type;
public:
    EPollEvent();
    EPollEvent(EPollEvent const &ref);
	EPollEvent &operator=(const EPollEvent &ref);
    ~EPollEvent();

    EPollEvent(epoll_event ev);

    int getFd() const;

    const epoll_event* raw() const;
	type getType() const;
	void setType(type _type);
};
