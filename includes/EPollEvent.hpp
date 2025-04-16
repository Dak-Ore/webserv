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
    EPollEvent(epoll_event ev);
    ~EPollEvent();

    int getFd() const;
    void setFd(int fd);

    epoll_event* raw();
    const epoll_event* raw() const;
};
