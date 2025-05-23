#include "Webserv.hpp"
#include "HttpClient.hpp"
#include "HttpRequest.hpp"
#include "Adress.hpp"
#include <algorithm>
#include <vector>

#define REQUEST_MAX_SIZE 8192

Webserv::Webserv(ConfigParser &parser) : _run(true)
{
	Socket socket;
	std::vector<int> ports;
	size_t count = parser.getServer().size();
	for (size_t i = 0; i < count; i++)
	{
		ServerConfig config = parser.getServer()[i];
		this->_servers.push_back(new Server(config));

		for (size_t i = 0; i < config.getAdress().size(); i++)
		{
			const Adress &adress = config.getAdress()[i];
			if (std::find(ports.begin(), ports.end(), adress.port()) != ports.end())
				continue;
			ports.push_back(adress.port());
			socket = Socket(adress.host_str(), adress.port_str());
			std::cout << "Server launched on \033[4mhttp://" << adress << "\033[0m" << std::endl;
			this->_sockets.push_back(socket);
			this->_epoll.addSocket(socket);
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

Server& Webserv::findServer(const HttpRequest &request, const HttpClient &client) const
{
	std::vector<Server *> matches;
	for (size_t i = 0; i < this->_servers.size(); i++)
	{
		Server *s = this->_servers[i];
		const std::vector<Adress> &adresses = s->_config.getAdress();
		if (std::find(adresses.begin(), adresses.end(), client.getServerAdress()) != adresses.end())
			matches.push_back(s);
	}
	if (!matches.empty())
	{
		std::string host = request.getHeader("Host");
		if (host.empty())
			return (*matches[0]);
		size_t pos = host.find(':');
		if (pos != std::string::npos)
			host = host.substr(0, pos);
		for (size_t i = 0; i < matches.size(); ++i)
		{
			const std::vector<std::string>& names = matches[i]->_config.getServerNames();
			if (std::find(names.begin(), names.end(), host) != names.end())
				return (*matches[i]);
		}
		if (!matches.empty())
			return (*matches[0]);
	}

	return (*this->_servers[0]);	
}

Config* Webserv::findConfig(const HttpRequest  &request, const HttpClient &client) const
{
	return (this->findServer(request, client).getConfig(request));
}

void Webserv::listen()
{
	while (this->_run)
	{
		const std::vector<EPollEvent> &events = this->_epoll.getEvents();
		for (std::vector<EPollEvent>::const_iterator event = events.begin(); event != events.end(); ++event)
		{
			std::cout << "EVENT :" << event->getFd() << std::endl;
			int fd = event->getFd();
			if (this->isServerSocket(fd))
				this->acceptClient(fd);
			else
			{
				HttpClient client = this->_client_map[fd];
				HttpRequest const request(client, *this);
				Server &s = this->findServer(request, client);
				if (!s.handleRequest(request, client))
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
		this->_epoll.addClient(client);
		this->_client_map[client.getFd()] = client;
	}
}

void Webserv::stop()
{
	this->_run = false;
}
