#include "ConfigParser.hpp"
#include "Server.hpp"
#include "EPoll.hpp"
#include "HttpClient.hpp"

#include <vector>
#include <map>

class Webserv
{
private:
    bool _run;
    EPoll _epoll;
    std::map<int, HttpClient> _client_map;
    std::vector<Server*> _servers;
	std::vector<Socket> _sockets;
	bool isServerSocket(int fd);
	Server* findServer(const HttpRequest  &request, const HttpClient &client);
	void acceptClient(int serverFd);
	HttpRequest readRequest(int fd);
	bool readHeaders(int fd, std::string& headers, std::string& body);
	bool readBody(int fd, std::string& body);
public:
    Webserv(ConfigParser &parser);
    ~Webserv();
    void listen();
    void stop();
};
