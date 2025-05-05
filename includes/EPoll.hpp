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
	/**
	 * @brief Returns the array of triggered events.
	 * @return Pointer to the array of EPollEvent.
	 */
	EPollEvent *getEvents();
	/**
	 * @brief Adds a listening socket to epoll for monitoring incoming connections.
	 * @param fd Listening socket file descriptor.
	 */
	void addSocket(int fd);
	/**
	 * @brief Adds a client socket (already accepted) to epoll for monitoring.
	 * @param fd Client socket file descriptor.
	 */
	void addClient(int fd);
	void remove(int fd, bool close_fd = true);
};

