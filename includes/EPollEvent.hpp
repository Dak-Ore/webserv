#pragma once

#include <sys/epoll.h>
#include <cstring>

enum event_type{
	UNKNOWN,
	IN,
	OUT
};

class EPollEvent
{
private:
    epoll_event _event;
	event_type _type;
public:
    EPollEvent();
    EPollEvent(EPollEvent const &ref);
	EPollEvent &operator=(const EPollEvent &ref);
    ~EPollEvent();

    EPollEvent(epoll_event ev);

    int getFd() const;

    const epoll_event* raw() const;
	event_type getType() const;
};
