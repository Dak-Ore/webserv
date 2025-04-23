#include "Webserv.hpp"

Webserv::Webserv(ConfigParser &parser) :
    _run(true)
{
	Socket *socket;
    size_t count = parser.getServer().size();
    for (size_t i = 0; i < count; i++)
    {
        ServerConfig config = parser.getServer()[i];
        this->_servers.push_back(new Server(this->_epoll, config));

		for (size_t i = 0; i < config.getAdress().size(); i++)
		{
			socket = new Socket(config.getAdress()[i].first, config.getAdress()[i].second);
			std::cout << "Server launched on " << config.getAdress()[i].first << ":" << config.getAdress()[i].second << std::endl;
			this->_sockets.push_back(socket);
			this->_epoll.addSocket(socket->getFd());
		}
    }
}

Webserv::~Webserv()
{
	for (size_t i = 0; i < this->_servers.size(); ++i)
		delete this->_servers[i];
	for (size_t i = 0; i < this->_sockets.size(); ++i)
		delete this->_sockets[i];
}

bool Webserv::isServerSocket(int fd)
{
	size_t n = this->_sockets.size();
	for (size_t i = 0; i < n; i++)
	{
		if (fd == this->_sockets[i]->getFd())
			return (true);
	}
	return (false);
}

Server *Webserv::findServer(int fd)
{
	if (this->isServerSocket(fd))	
		return (this->_servers[0]);
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
				int clientFd = this->acceptClient(fd);
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

int Webserv::acceptClient(int serverFd)
{
	int client_fd = ::accept(serverFd, NULL, NULL);
	if (client_fd != -1)
		this->_epoll.addClient(client_fd);
	return (client_fd);
}

void Webserv::stop()
{
	this->_run = false;
}
