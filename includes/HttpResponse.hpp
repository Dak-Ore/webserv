#pragma once

#include "CGI.hpp"
#include "HttpMessage.hpp"

#include <string>

enum cookie_options
{
	COOKIE_DOMAIN,
	COOKIE_EXPIRES,
	COOKIE_HTTPONLY, // no value
	COOKIE_MAX_AGE,
	COOKIE_PARTITIONNED,
	COOKIE_PATH,
	COOKIE_SECURE, // no value
	COOKIE_SAME_SITE
};

class HttpClient;

class HttpResponse : HttpMessage
{
private:
	std::string _buffer;
	int 	_status_code;
	int 	_bodyFd;
	bool	_sending;
	bool	_done;
	// bool _keep_alive;
	void	closeBody();
	void 	_setContentType(const std::string& file_name);
	void 	_setHeader(const std::string &key, const std::string &value);
	std::string _getCookieHeader();
public:
	HttpResponse(int status_code = 200);
	~HttpResponse();

	/**
	 * @brief Get the reason phrase for a given status code.
	 * @param code HTTP status code.
	 * @return Corresponding reason phrase as string (e.g., "Not Found").
	 */
	static std::string getReason(int code);

	/**
	 * @brief Get the reason phrase for this response's status code.
	 * @return Reason string for the current code.
	 */
	std::string getReason() const;

	/**
	 * @brief Convert the response (headers + body) to a string.
	 * @return Full HTTP response string.
	 */
	std::string toString();

	/**
	 * @brief Set the response body as a string.
	 * @param body Content to include in the response body.
	 */
	void setBody(const std::string &body);

	/**
	 * @brief Set the response body to be read from a file.
	 * @param file_name Path to the file.
	 * @return true if file was successfully opened; false otherwise.
	 */
	bool setBodySource(const std::string &file_name);

	bool hasBody();

	/**
	 * @brief Set the HTTP status code.
	 * @param code HTTP status code.
	 */
	void setCode(int code);

	/**
	 * @brief Set a header key-value pair.
	 * @param key Header name.
	 * @param value Header value.
	 */
	void setHeader(const std::string &key, const std::string &value);

	/**
	 * @brief Set a cookie in the response.
	 * @param key Cookie name.
	 * @param value Cookie value.
	 * @param options Map of cookie attributes (e.g., Path, Secure).
	 */
	void setCookie(const std::string &key, const std::string &value, std::map<cookie_options, std::string> options = std::map<cookie_options, std::string>());

	/**
	 * @brief Get the current response status code.
	 * @return Status code.
	 */
	int getCode() const;

	bool isOK() const;
	bool isDone() const;
	std::string &read();
};
