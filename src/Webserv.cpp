#include "Webserv.hpp"
#include "HttpClient.hpp"
#include "HttpRequest.hpp"
#include "Adress.hpp"
#include <algorithm>
#include <unistd.h>
#include <vector>
#include <ctime>
#include <typeinfo>

#define REQUEST_MAX_SIZE 8192

static void logRequest(const HttpRequest &request)
{
	std::cout << request.getMethod() << " - " << request.getHeader("Host")  << request.getPath() << std::endl;
}

static void logResponse(const HttpResponse &response)
{
	std::cout << "\t- RESPONSE: " << response.getCode() << " " << response.getReason() << std::endl;
}

Webserv::Webserv(ConfigParser &parser) : _run(true)
{
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
			ServerSocket socket(adress);
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
bool Webserv::isClientSocket(int fd)
{
	return (this->_client_map.find(fd) != this->_client_map.end());
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

Config* Webserv::findConfig(const HttpClient &client) const
{
	const HttpRequest &request = client.request();
	return (this->findServer(request, client).getConfig(request));
}

void Webserv::acceptClient(int serverFd)
{
	HttpClient client(serverFd, *this);
	if (client.getFd() != -1)
	{
		this->_epoll.addClient(client);
		this->_client_map[client.getFd()] = client;
	}
}

void Webserv::listen()
{
	// const int TIMEOUT_SECONDS = 10;
	// static time_t lastTimeoutCheck = std::time(NULL);
	
	while (this->_run)
	{
		const std::vector<EPollEvent> &events = this->_epoll.getEvents();
		for (std::vector<EPollEvent>::const_iterator event = events.begin(); event != events.end(); ++event)
		{
			int fd = event->getFd();
			if (this->isServerSocket(fd))
				this->acceptClient(fd);
			else if (this->isClientSocket(fd))
			{
				event_type type = event->getType();
				HttpClient &client = this->_client_map[fd];
				if (type == IN)
				{
					if (client.readRequest()) // ready ?
					{
						this->handleRequest(client);
						logResponse(client.response());
						this->_epoll.setOut(client);
					}
				}
				else if (type == OUT)
				{
					client.send();
					if (client.response().isDone())
					{
						this->_epoll.setIn(client);
						this->_epoll.remove(client.getFd());
						this->_client_map.erase(fd);
					}
				}
			}
		}
		// time_t now = std::time(NULL);
		// if (now - lastTimeoutCheck >= 2)
		// {
		// 	lastTimeoutCheck = now;

		// 	for (std::map<int, HttpClient>::iterator it = this->_client_map.begin(); it != this->_client_map.end();)
		// 	{
		// 		time_t last = it->second.request().getCreatedAt();
		// 		if (now - last > TIMEOUT_SECONDS)
		// 		{
		// 			int fd = it->first;
		// 			this->_epoll.remove(fd);
		// 			this->_client_map.erase(it++);
		// 		}
		// 		else
		// 			it++;
		// 	}
		// }
	}
}

void Webserv::handleRequest(HttpClient &client)
{
	HttpRequest &request = client.request();
	HttpResponse &response = client.response();
	if (request.empty())
		return ;
	logRequest(request);
	Config* config = request.getConfig();
	LocationConfig *location = dynamic_cast<LocationConfig *>(config);

	if (this->handleRedirect(response, location))
		return ;
	if (!request.isValid())
		response.setCode(request.getErrorCode());
	else
	{
		std::string relativePath = config->getRelativePath(request.getPath());
		std::string file_path = utils::joinPath(config->getRoot(), relativePath);
		if (utils::isDirectory(file_path))
		{
			if (config->getAutoIndex())
			{
				this->sendAutoindex(request, response, file_path);
				return ;
			}
			else
				file_path = config->findIndex(relativePath);
		}
		std::cout << "\t- FILE: " <<  file_path << std::endl;
		if (this->handleCgi(client, location, file_path))
			return ;
		response.setBodySource(file_path);
	}
	this->handleErrorPages(response, config);
	return ;
}

bool Webserv::handleCgi(HttpClient &client, LocationConfig *location, std::string file_path)
{
	std::cout << "HERE" << std::endl;
	if (!location)
		return (false);
	std::cout << file_path << std::endl;
	const CGI *cgi = location->getCgi(file_path);
	if (!cgi)
	{
		std::cout << "dont work" << std::endl;	
		return (false); 
	}
	int data_fd;
	cgi->execute(data_fd, file_path, file_path, client);
	std::string const& body(client.request().getBody());
	write(data_fd, body.c_str(), body.size());
	close(data_fd);
	return (true);	
}

bool Webserv::handleRedirect(HttpResponse &response, LocationConfig *location)
{
	if (!location || !location->getHasRedirection())
		return (false);
	response.setCode(location->getRedirection().first);
	response.setHeader("Location", location->getRedirection().second);
	return (true);
}

void Webserv::sendAutoindex(const HttpRequest &request, HttpResponse &response, const std::string &directory)
{
	std::string html = utils::generateAutoIndex(directory, request.getPath());
	response.setBody(html);
}

void Webserv::handleErrorPages(HttpResponse &response, Config *config)
{
	std::map<int, std::string>::const_iterator it = config->getErrorPages().find(response.getCode());
	if (!response.hasBody() && it != config->getErrorPages().end())
		response.setBodySource(it->second);
	if (!response.hasBody() && !response.isOK())
		response.setBody(utils::generateDefaultError(response.getCode()));
}

void Webserv::stop()
{
	this->_run = false;
}