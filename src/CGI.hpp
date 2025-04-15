#pragma once

#include <map>
#include <string>

/**
 * Is connected to a CGI script
 * ??? TODO
 */
class CGI
{

public:
	// TODO remove? static bool isValidMetavars(std::map<std::string, std::string> metavars);

private:
	// default constructor, results in an undefined object.
	CGI();
	// copy constructor
	CGI(CGI const&);
	// copy operator
	CGI& operator=(CGI const&);

public:
	/**
	 * Create a CGI (TODO)
	 */
	CGI(char const* pathname, char const* const argv[] = {});

	virtual ~CGI();

	/**
	 * TODO
	 * Execute a request.
	 * - 'metavars': the meta-variables to send to the script.
	 *   - AUTH_TYPE: optional, probably unnecessary.
	 *   - CONTENT_LENGTH: number of octets of body, or "" or unset if body is
	 *     empty. To set inside.
	 *   - CONTENT_TYPE: if body non-empty, internet media type of body.
	 *     if unset, the script could try to determine it itself or it could not
	 *     and we'll have to suffer from its wrath.
	 *     MUST be defined if the user sends an HTTP Content-Type field.
	 *   - GATEWAY_INTERFACE: "CGI/" +digit "." +digit. NO leading zero.
	 *     The version used is 1.1.
	 *   - PATH_INFO: probably unnecessary
	 *   - PATH_TRANSLATED: same
	 *   - QUERY_STRING: after the "?" in an url. MUST be set, even if it's "".
	 *   - REMOTE_ADDR: client address (ipv4 or ipv6)
	 *   - REMOTE_HOST: just use REMOTE_ADDR
	 *   - REMOTE_IDENT: who cares
	 *   - REMOTE_USER: for auth, probably unnecessary
	 *   - REQUEST_METHOD: "GET" | "POST" ...
	 *   - SCRIPT_NAME: path of the script, or "". MUST be set. I don't know if obligatory.
	 *   - SERVER_NAME: hostname or ipv4/6 (if ipv6: "[" ipv6 "]"). If several possible
	 *     value, see the request's Host header field.
	 *   - SERVER_PORT: obvious (MUST be set, even if 80).
	 *   - SERVER_PROTOCOL: "HTTP/" +digit "." +digit ("HTTP/1.1"?) or "INCLUDED"
	 *   - SERVER_SOFTWARE: use eponymous constant
	 *   - HTTP_*: from request header (uppercased, "-" -> "_")
	 *     If several request headers, put them in one having the same meaning.
	 *     Same if several lines.
	 *     Remove all authentification information, and informations already
	 *     available in others metavars.
	 * 
	 * Note: if REQUEST_METHOD == "HEAD", discard any body content given by the script.
	 * 
	 * TODO add timeout
	 */
	void execute(int inout[2], std::map<std::string, std::string> metavars, std::string body);

private:
	std::string pathname;
	char* const* argv;
};

/*

CGI-Response = document-response | local-redir-response | client-redir-response
	| client-redirdoc-response
document-response = Content-Type [ Status ] *other-field NL response-body
	(If status is ommitted, 200 OK is assumed).
local-redir-response = local-Location NL
client-redir-response = client-Location *extension-field NL
	(302 Found)
client-redirdoc-response = client-Location Status Content-Type *other-field NL response-body
	(Status MUST be 302 Found)

header-field = CGI-field | other-field
CGI-field = Content-Type | Location | Status
other-field = protocol-field | extension-field
protocol-field = generic-field
extension-field = generic-field
generic-field = field-name ":" *WHITESPACE [ field-value ] NL
field-name = token
field-value = *( *WHITESPACE ( field-content | LWSP ) *WHITESPACE
field-content = *( token | separator | quoted-string )

Content-Type = ... media-type NL
  (mandatory)
  (send as is to the client)

Location = local-Location | client-Location
client-Location = "Location:" fragment-URI NL
local-Location = "Location:" local-pathquery NL
fragment-URI = absoluteURI [ "#" fragment ]
fragment = *uric
local
ugh

absoluteURI uric media-type LWSP Location client-Location local-Location Status NL response-body client-redirdoc-response


TODO The server MUST make any appropriate modifications to the script's
   output to ensure that the response to the client complies with the
   response protocol version. ????
   https://datatracker.ietf.org/doc/html/rfc3875#section-6.2.1
*/
