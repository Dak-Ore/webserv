#include "Webserv.hpp"

Webserv::Webserv(ConfigParser &parser) :
    _run(true)
{
    size_t count = parser.getServer().size();
    for (size_t i = 0; i < count; i++)
    {
        ServerConfig config = parser.getServer()[i];
        this->_servers.push_back(new Server(this->_epoll, config));

    }
}

Webserv::~Webserv()
{
	for (size_t i = 0; i < this->_servers.size(); ++i)
		delete this->_servers[i];
}

Server *Webserv::findServer(int fd)
{
	for (size_t i = 0; i < this->_servers.size(); i++)
	{
		Server *s = this->_servers[i];
		if (s->isServerSocket(fd))	
			return (s);
	}	
	return (NULL);
}

void Webserv::listen()
{
	while (this->_run)
	{
		EPollEvent* events = this->_epoll.getEvents();
		for (int i = 0; i < EPOLL_MAX_EVENTS; ++i)
		{
			int fd = events[i].getFd();
			if (fd == 0)
				continue;
			Server *s = this->findServer(fd);
			if (s)
			{
				int clientFd = s->acceptClient(fd);
				if (clientFd != -1)
					this->_client_map[clientFd] = s;
			}
			else
			{
				s = _client_map[fd];
				HttpRequest const request = s->readRequest(fd);
				if (!s->handleRequest(request, fd))
					this->_epoll.remove(fd);
			}
		}
	}
}

void Webserv::stop()
{
	this->_run = false;
}
