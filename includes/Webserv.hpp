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
public:
    Webserv(ConfigParser &parser);
    ~Webserv();
	bool isServerSocket(int fd);
	Server* findServer(int fd);
    void listen();
	void acceptClient(int serverFd);
    void stop();
	HttpRequest readRequest(int fd);
	bool readHeaders(int fd, std::string& headers, std::string& body);
	bool readBody(int fd, std::string& body);
};
