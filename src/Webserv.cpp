#include "Webserv.hpp"
#include "HttpClient.hpp"
#include <algorithm>

Webserv::Webserv(ConfigParser &parser) : _run(true)
{
	Socket socket;
	size_t count = parser.getServer().size();
	for (size_t i = 0; i < count; i++)
	{
		ServerConfig config = parser.getServer()[i];
		this->_servers.push_back(new Server(this->_epoll, config));

		for (size_t i = 0; i < config.getAdress().size(); i++)
		{
			socket = Socket(config.getAdress()[i].host_str(), config.getAdress()[i].port_str());
			std::cout << "Server launched on " << config.getAdress()[i].str() << std::endl;
			this->_sockets.push_back(socket);
			this->_epoll.addSocket(socket.getFd());
		}
	}
}

Webserv::~Webserv()
{
	for (size_t i = 0; i < this->_servers.size(); ++i)
		delete this->_servers[i];
	for (size_t i = 0; i < this->_sockets.size(); ++i)
		this->_sockets[i].close();
}

bool Webserv::isServerSocket(int fd)
{
	size_t n = this->_sockets.size();
	for (size_t i = 0; i < n; i++)
	{
		if (fd == this->_sockets[i].getFd())
			return (true);
	}
	return (false);
}

Server *Webserv::findServer(int fd)
{
	std::vector<Server *> v;
	HttpClient client = this->_client_map[fd];
	for (size_t i = 0; i < this->_servers.size(); i++)
	{
		Server *s = this->_servers[i];
		const std::vector<Adress> &adresses = s->_config.getAdress();
		if (std::find(adresses.begin(), adresses.end(), client.getAdress()) != adresses.end())
			v.push_back(s);
	}
	if (v.size() == 0)
		return (this->_servers[0]);
	else
		return (v[0]);
}

void Webserv::listen()
{
	while (this->_run)
	{
		EPollEvent *events = this->_epoll.getEvents();
		for (int i = 0; i < EPOLL_MAX_EVENTS; ++i)
		{
			int fd = events[i].getFd();
			if (fd == 0)
				continue;
			if (this->isServerSocket(fd))
				this->acceptClient(fd);
			else
			{
				Server *s = this->findServer(fd);
				HttpRequest const request = s->readRequest(fd);
				if (!s->handleRequest(request, fd))
					this->_epoll.remove(fd);
			}
		}
	}
}

void Webserv::acceptClient(int serverFd)
{
	HttpClient client(serverFd);
	if (client.getFd() != -1)
	{
		this->_epoll.addClient(client.getFd());
		this->_client_map[client.getFd()] = client;
	}
}

void Webserv::stop()
{
	this->_run = false;
}
