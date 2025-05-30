#pragma once

#include <set>
#include <map>
#include "EPollEvent.hpp"
#include "CGI.hpp"
#include <vector>

#define EPOLL_MAX_EVENTS 50

class Socket;
class HttpClient;


class EPoll
{
private:
	int _fd;
	std::set<int> _fds;
	std::map<int, bool> _cgis;
	std::vector<EPollEvent> _events;
	static int setNonBlocking(int fd);
	void wait();
	void add(int fd, int flags);
	void update(int fd, int flags);
public:
	EPoll();
	~EPoll();
	/**
	 * @brief Returns the array of triggered events.
	 * @return Pointer to the array of EPollEvent.
	 */
	const std::vector<EPollEvent> &getEvents();
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
	void addNewCgi(const CGI::Running &cgiProcess);
	void setCgiReady(const CGI::Running &cgiProcess);
	void remove(int fd, bool close_fd = true);
	void setIn(const HttpClient &client);
	void setOut(const HttpClient &client);
	void remove(const Socket &socket, bool close_fd = true);
	void remove(const HttpClient &client, bool close_fd = true);
	void remove(const CGI::Running &cgiProcess, bool close_fd = true);
};

