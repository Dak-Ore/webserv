#include "ConfigParser.hpp"
#include "Server.hpp"
#include "EPoll.hpp"

#include <vector>
#include <map>

class Webserv
{
private:
    bool _run;
    EPoll _epoll;
    std::map<int, Server *> _client_map;
    std::vector<Server *> _servers;
	std::vector<Socket*> _sockets;
public:
    Webserv(ConfigParser &parser);
    ~Webserv();
	bool isServerSocket(int fd);
    Server *findServer(int fd);
    void listen();
	int acceptClient(int serverFd);
    void stop();
};
