#pragma once

#include <set>
#include "EPollEvent.hpp"

#define EPOLL_MAX_EVENTS 10

class Socket;
class HttpClient;

class EPoll
{
private:
	int _fd;
	std::set<int> _fds;
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
	void addSocket(const Socket &socket);
	/**
	 * @brief Adds a client socket (already accepted) to epoll for monitoring.
	 * @param fd Client socket file descriptor.
	 */
	void addClient(const HttpClient &client);
	void remove(int fd, bool close_fd = true);
};

