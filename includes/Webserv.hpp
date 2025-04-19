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
public:
    Webserv(ConfigParser &parser);
    ~Webserv();
    Server *findServer(int fd);
    void listen();
    void stop();
};
