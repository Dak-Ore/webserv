#include "Webserv.hpp"
#include "HttpClient.hpp"
#include "HttpRequest.hpp"
#include "Adress.hpp"
#include "UploadHandler.hpp"
#include <algorithm>
#include <unistd.h>
#include <vector>
#include <ctime>
#include <typeinfo>
#include <filesystem>

#define TIMEOUT_SECONDS 2

static void logRequest(const HttpRequest &request)
{
	std::cout << request.getMethod() << " - " << request.getHeader("Host") << request.getPath() << std::endl;
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
	return (this->_clients.find(fd) != this->_clients.end());
}

Server &Webserv::findServer(const HttpRequest &request, const HttpClient &client) const
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
			const std::vector<std::string> &names = matches[i]->_config.getServerNames();
			if (std::find(names.begin(), names.end(), host) != names.end())
				return (*matches[i]);
		}
		if (!matches.empty())
			return (*matches[0]);
	}

	return (*this->_servers[0]);
}

Config *Webserv::findConfig(const HttpClient &client) const
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
		this->_clients[client.getFd()] = client;
	}
}

void Webserv::removeClient(int fd)
{
	this->_epoll.remove(fd);
	this->_clients.erase(fd);
}

void Webserv::removeClient(const HttpClient &client)
{
	this->removeClient(client.getFd());
}

void Webserv::listen()
{
	while (this->_run)
	{
		this->timeout();
		const std::vector<EPollEvent> &events = this->_epoll.getEvents();
		for (std::vector<EPollEvent>::const_iterator event = events.begin(); event != events.end(); ++event)
		{
			int fd = event->getFd();
			EPollEvent::type type = event->getType();
			if (this->isServerSocket(fd))
				this->acceptClient(fd);
			else if (this->isClientSocket(fd))
			{
				if (this->_clients.find(fd) == this->_clients.end())
				{
					this->_epoll.remove(fd);
					continue;
				}
				HttpClient &client = this->_clients[fd];
				if (type == EPollEvent::IN)
				{
					try
					{
						if (client.readRequest()) // ready ?
						{
							this->handleRequest(client);
							if (!client.response().hasCgi())
							{
								logResponse(client.response());
								this->_epoll.setOut(client);
							}
						}
					}
					catch (const Socket::closedSocketException &e)
					{
						this->removeClient(client);
					}
				}
				else if (type == EPollEvent::OUT)
				{
					try
					{
						client.send();
					}
					catch (const Socket::closedSocketException &e)
					{
						this->removeClient(client);
						continue;
					}

					if (client.response().isDone())
						this->removeClient(client);
				}
			}
			else if (type == EPollEvent::CGI)
			{
				this->readFromCgi(*this->_CGIs[fd].first, *this->_CGIs[fd].second);
			}
		}
	}
}

void Webserv::timeout()
{
	static time_t lastTimeoutCheck = std::time(NULL);

	time_t now = std::time(NULL);
	if (now - lastTimeoutCheck >= 2)
	{
		lastTimeoutCheck = now;

		for (std::map<int, HttpClient>::iterator it = this->_clients.begin(); it != this->_clients.end();)
		{
			time_t last = it->second.request().getCreatedAt();
			if (now - last > TIMEOUT_SECONDS)
			{
				int fd = it->first;
				HttpClient &client = it->second;
				HttpResponse &response = client.response();
				if (!response.isSending())
				{
					response.clearbody();
					response.setCode(408);
					if (client.response().hasCgi())
						try
						{
							this->_epoll.addClient(client);
						}
						catch (const std::exception &e)
						{
						}

					this->_epoll.setOut(client);
				}
				else
					this->removeClient(fd);
				it++;
			}
			else
				it++;
		}
	}
}

void Webserv::handleRequest(HttpClient &client)
{
	HttpRequest &request = client.request();
	HttpResponse &response = client.response();
	UploadHandler upload(request, request.getConfig());
	if (request.empty())
		return;
	logRequest(request);
	Config *config = request.getConfig();
	LocationConfig *location = dynamic_cast<LocationConfig *>(config);

	if (this->handleRedirect(response, location))
		return ;
	if (request.getMethod() == "DELETE")
        this->handleDeleteRequest(request, response);
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
				return;
			}
			else
				file_path = config->findIndex(relativePath);
		}
		if (this->handleCgi(client, location, file_path))
			return ;
		if (upload.hasMultipart())
		{
			response.setCode(upload.getError());
			if (upload.getError() != 200)
				response.setBody(response.getReason(response.getCode()));
			else
				response.setBody("File uploaded");
			return ;
		}
		response.setBodySource(file_path);
	}
	this->handleErrorPages(response, config);
}

void Webserv::handleDeleteRequest(HttpRequest &request, HttpResponse &response)
{
	
	std::string filePath = request.getPath();
	// Check if we are in location
	size_t pos = filePath.find_last_of("/");
	if (pos != std::string::npos)
		filePath = filePath.substr(pos + 1);

    std::string fullPath = utils::joinPath(request.getConfig()->getRoot(), filePath);
	std::cout << fullPath << " : " << filePath << std::endl;
    // check if file exist
    if (!utils::fileExists(fullPath)) {
        response.setCode(404);
        return;
    }
    // delete file
    if (remove(fullPath.c_str()) != 0) {
        response.setCode(500);
        return;
    }
    response.setCode(200);
	response.setBody("File deleted");
}


bool Webserv::handleCgi(HttpClient &client, LocationConfig *location, std::string file_path)
{
	if (!location)
		return (false);
	const CGI *cgi = location->getCgi(file_path);
	if (!cgi)
		return (false);
	CGI::Running *cgiProcess = cgi->execute(file_path, client);
	this->_CGIs[cgiProcess->getFd()] = std::pair<CGI::Running *, HttpClient *>(cgiProcess, &client);
	this->_epoll.addNewCgi(*cgiProcess);
	client.response().useCgi(*cgiProcess);
	this->_epoll.remove(client, false);
	return (true);
}

void Webserv::readFromCgi(CGI::Running &cgiProcess, HttpClient &client)
{
	HttpResponse &response = client.response();
	try
	{
		if (!cgiProcess.read())
		{
			this->_epoll.remove(cgiProcess, false);
			this->_CGIs.erase(cgiProcess.getResponseBodyFd());
			this->_epoll.addClient(client);
			this->_epoll.setOut(client);
			return;
		}
	}
	catch (const utils::execve_error &e)
	{
		this->_epoll.remove(cgiProcess, false);
		this->_CGIs.erase(cgiProcess.getFd());
		return;
	}
	if (!response.isCgiHeaderOk() && cgiProcess.isHeadComplete())
	{
		CGI::Running::ResponseHead responseHead = cgiProcess.getResponseHead();
		response.setCode(responseHead.status_code);
		// set the header of the response
		for (std::map<std::string, std::string>::iterator it = responseHead.fields.begin(); it != responseHead.fields.end(); it++)
		{
			response.setHeader(utils::trim(it->first), it->second);
		}
		response.setBodySource(cgiProcess.getResponseBodyFd());
		this->_epoll.setCgiReady(cgiProcess);
		int oldFd = cgiProcess.getFd();
		int newFd = cgiProcess.getResponseBodyFd();
		this->_CGIs[newFd] = this->_CGIs[oldFd];
		this->_CGIs.erase(oldFd);
		response.cgiHeaderOk();
	}
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