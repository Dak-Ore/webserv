#pragma once

#include <sys/epoll.h>
#include <cstring>

class EPollEvent
{
private:
    epoll_event _event;
public:
    EPollEvent();
    EPollEvent(EPollEvent const &ref);
	EPollEvent &operator=(const EPollEvent &ref);
    ~EPollEvent();

    EPollEvent(epoll_event ev);

    int getFd() const;

    const epoll_event* raw() const;
};
