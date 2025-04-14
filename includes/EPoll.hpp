#define EPOLL_MAX_EVENTS 10

class EPoll
{
private:
	int _fd;
	epoll_event _events[EPOLL_MAX_EVENTS];
	int setNonBlocking(int fd);
	int wait();
	void add(int fd, int flags);
public:
	EPoll();
	~EPoll();
	void addSocket(int fd);
	void addClient(int fd);
	epoll_event *getEvents();
	void processEvents(int (*func)(EPoll &e, int fd));
};

