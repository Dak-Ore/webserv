#include "Webserv.hpp"
#include "HttpClient.hpp"
#include "Adress.hpp"
#include <algorithm>
#include <vector>

Webserv::Webserv(ConfigParser &parser) : _run(true)
{
	Socket socket;
	std::vector<int> ports;
	size_t count = parser.getServer().size();
	for (size_t i = 0; i < count; i++)
	{
		ServerConfig config = parser.getServer()[i];
		this->_servers.push_back(new Server(this->_epoll, config));

		for (size_t i = 0; i < config.getAdress().size(); i++)
		{
			const Adress &adress = config.getAdress()[i];
			if (std::find(ports.begin(), ports.end(), adress.port()) != ports.end())
				continue;
			ports.push_back(adress.port());
			socket = Socket(adress.host_str(), adress.port_str());
			std::cout << "Server launched on " << adress << std::endl;
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

Server* Webserv::findServer(const HttpRequest &request, const HttpClient &client)
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
			return (matches[0]);
		size_t pos = host.find(':');
		if (pos != std::string::npos)
			host = host.substr(0, pos);
		for (size_t i = 0; i < matches.size(); ++i)
		{
			const std::vector<std::string>& names = matches[i]->_config.getServerNames();
			if (std::find(names.begin(), names.end(), host) != names.end())
				return matches[i];
		}
		if (!matches.empty())
			return (matches[0]);
	}

	return (this->_servers[0]);	
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
				HttpRequest const request = this->readRequest(fd);
				HttpClient client = this->_client_map[fd];
				Server *s = this->findServer(request, client);
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
		this->_epoll.addClient(client);
		this->_client_map[client.getFd()] = client;
	}
}

void Webserv::stop()
{
	this->_run = false;
}

HttpRequest Webserv::readRequest(int fd)
{
	std::string headers, body;

	this->readHeaders(fd, headers, body);
	HttpRequest request(headers);
	this->readBody(fd, body);
	return (request);
}

bool Webserv::readHeaders(int fd, std::string& headers, std::string& body)
{
	char buffer[1024];
	int bytes;
	size_t header_limit;

	while (true)
	{
		bytes = ::recv(fd, buffer, sizeof(buffer), 0);
		if (bytes <= 0)
			return (false);
		headers.append(buffer, bytes);

		header_limit = headers.find("\r\n\r\n");
		if (header_limit != std::string::npos)
		{
			body = headers.substr(header_limit + 4);
			headers.erase(header_limit, 4);
			break ;
		}
	}
	return (true);
}

bool Webserv::readBody(int fd, std::string& body)
{
	char buffer[1024];
	int bytes;

	while (true)
	{
		bytes = ::recv(fd, buffer, sizeof(buffer), 0);
		if (bytes <= 0)
			return (false);
		body.append(buffer, bytes);
	}
	return (true);
}