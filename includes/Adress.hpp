#pragma once

#include <netdb.h>
#include <string>
#include <ostream>

class Socket;

class Adress
{
private:
	int _host;
	int _port;
	sockaddr_in  _sockaddr;
	socklen_t    _addrlen;
public:
	/**
	 * @brief Converts a host integer to a human-readable string.
	 * @param host IP address in network byte order.
	 * @return IP address as a dotted string.
	 */
	static std::string hostToString(int host);

	Adress();
	Adress(const Adress &ref);
	const Adress &operator=(const Adress &ref);
	~Adress();
	bool operator==(const Adress &ref) const;

	/**
	 * @brief Constructs an Adress from an existing Socket.
	 * @param socket Socket pointer
	 */
	Adress(Socket *socket);

	Adress(const struct sockaddr_in &addrinfo);

	/**
	 * @brief Constructs an Adress from host and port (as integers).
	 * @param host IP address in network byte order.
	 * @param port Port number in network byte order.
	 */
	Adress(int host, int port);

	/**
	 * @brief Constructs an Adress from host and port (as strings).
	 * @param host IP address or hostname.
	 * @param port Port number as string.
	 */
	Adress(const std::string &host, const std::string &port);

	int host() const;
	int port() const;
	std::string host_str() const;
	std::string port_str() const;
	/// Returns a string representation (host:port).
	std::string str() const;

	/**
	 * @brief Creates a socket from the current Adress.
	 * @return A socket file descriptor, or throws on failure.
	 */
	int createSocket() const;

	/**
	 * @brief Binds the current Adress to the given file descriptor.
	 * @param fd Socket file descriptor.
	 * @return True on success, false otherwise.
	 */
	bool bind(int fd) const;
};

std::ostream& operator<<(std::ostream& os, const Adress &ref);