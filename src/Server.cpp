#include "Server.hpp"

#include <stdexcept>
#include <netdb.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <iostream>
#include <stdio.h>
#include <cstring>

int setNonBlocking(int fd) {
	int flags = fcntl(fd, F_GETFL, 0);
	return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

Server::Server()
{
	this->init("localhost", "80");
}

Server::Server(std::string hostname, std::string service)
{
	this->init(hostname, service);
}

void Server::init(std::string hostname, std::string service)
{
	this->sockets.push_back(new Socket(hostname, service));
	setNonBlocking(this->sockets[0]->getFd());
}

Server::~Server()
{
		for (size_t i = 0; i < sockets.size(); ++i)
			delete this->sockets[i];
}

void Server::listen()
{
	int fd = this->sockets[0]->getFd();
	const int MAX_EVENTS = 10;
	const int epoll_fd = epoll_create1(0);
	epoll_event ev;
	memset(&ev, 0, sizeof(ev));
	ev.events = EPOLLIN;
	ev.data.fd = fd;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev))
	{
		perror("a");
		throw std::runtime_error("epoll_ctl failed");
	}
	epoll_event events[MAX_EVENTS];
	while (true)
	{
		int n = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
		for (int i = 0; i < n; ++i) {
			if (events[i].data.fd == fd)
			{
				// Nouveau client
				int client_fd = accept(fd, NULL, NULL);
				if (client_fd == -1)
				{
					std::cerr << "accept failed" << std::endl;
					continue;
				}
				setNonBlocking(client_fd);
				ev.events = EPOLLIN | EPOLLET;
				ev.data.fd = client_fd;
				epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev);
				std::cout << "Nouveau client connecté\n";
			}
			else
			{
				// Client envoie des données
				char buf[1024];
				int count = read(events[i].data.fd, buf, sizeof(buf));
				if (count <= 0) {
					close(events[i].data.fd);
					std::cout << "Client déconnecté\n";
				} else {
					std::cout << "Reçu : " << std::string(buf, count) << "\n";

					std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";
					write(events[i].data.fd, response.c_str(), response.size());
					close(events[i].data.fd); // Pas de keep-alive dans cet exemple
				}
			}
		}
	}
}

std::string Server::readRequest(int fd)
{
	std::string request;
	char buffer[1024];
	while (true) {
		int bytes = recv(fd, buffer, sizeof(buffer), 0);
		if (bytes <= 0) break;
		request.append(buffer, bytes);

		if (request.find("\r\n\r\n") != std::string::npos) {
			// Fin des headers atteinte
			break;
		}
	}
	return (request);
}