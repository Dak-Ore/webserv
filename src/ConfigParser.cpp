#include "ConfigParser.hpp"
#include "Server.hpp"
#include "utils.hpp"
#include <CGI.hpp>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>

ConfigParser::CgiBuilder::CgiBuilder()
{
	this->clear();
}

CGI ConfigParser::CgiBuilder::toCgi()
{
	if (this->extension == "")
		throw std::runtime_error("this shouldn't happen.");
	if (this->_binary == "")
		throw std::runtime_error("'binary' must be defined in cgi block.");

	std::vector<std::string> argv;
	argv.push_back(this->_binary);
	if (this->_pass_script_through_argv)
		argv.push_back("%f");
	return CGI(argv, this->extension);
}

void ConfigParser::CgiBuilder::clear()
{
	this->extension = "";
	this->_binary = "";
	this->_pass_script_through_argv = true;
	this->_pass_script_through_argv_set = false;
}

void ConfigParser::CgiBuilder::read(std::string line)
{
	if (utils::startswith(line, "binary ")) {
		if (this->_binary != "")
			throw std::runtime_error("'binary' was defined several times.");
		std::string value = utils::smartSubstr(line, "binary ", ";");
		if (!utils::startswith(value, "/"))
			throw std::runtime_error("the value of 'binary' must be an absolute path.");
		this->_binary = value;
	}
	else if (utils::startswith(line, "pass_script_through_argv ")) {
		if (this->_pass_script_through_argv_set)
			throw std::runtime_error("'pass_script_through_argv' was defined several times.");
		this->_pass_script_through_argv_set = true;
		std::string value = utils::smartSubstr(line, "pass_script_through_argv ", ";");
		if (value == "true")
			this->_pass_script_through_argv = true;
		else if (value == "false")
			this->_pass_script_through_argv = false;
		else
			throw std::runtime_error("'pass_script_through_argv' takes either 'true' or 'false'.");
	}
	else {
		throw std::runtime_error("unknown parameter in cgi block.");
	}
}

ConfigParser::ConfigParser(File config)
{
	std::string line;
	std::string serverContent;
	std::string locationContent;
	std::vector<std::pair<std::string, std::vector<CGI> > > locationBlocks;
	std::vector<CGI> locationCgis;
	CgiBuilder cgiContent;
	std::stack<Block> blockstack;
	Block waitBrace = NONE;

	for (size_t i = 0; i < config.getLineNumber(); ++i) {
		line = utils::trim(config.getLine(i));
		if (line.empty() || line[0] == '#')
			continue;
		Block currentBlock = blockstack.size() > 0 ? blockstack.top() : NONE;
		if (processServerStart(line, waitBrace, blockstack)) {
			if (currentBlock != NONE)
				throw std::runtime_error("server block must be at the root of the config file.");
			continue;
		}
		if (processLocationStart(line, waitBrace, blockstack, locationContent))
		{
			if (currentBlock != SERVER)
				throw std::runtime_error("location block must be in a server block.");
			continue;
		}
		if (processCgiStart(line, waitBrace, blockstack, cgiContent))
		{
			if (currentBlock != LOCATION)
				throw std::runtime_error("cgi block must be in a location block.");
			continue;
		}
		if (processOpeningBrace(line, waitBrace, blockstack))
			continue;

		if (waitBrace || line == "{")
			throw std::runtime_error("Invalid config file (brace issue)");

		if (line == "}") {
			if (blockstack.size() == 0)
				std::runtime_error("closing bracket without opening bracket.");
			Block block = blockstack.top();
			blockstack.pop();
			switch (block) {
			case CGIBLOCK:
				locationCgis.push_back(cgiContent.toCgi());
				cgiContent.clear();
				break;
			case LOCATION:
				locationBlocks.push_back(std::pair<std::string, std::vector<CGI> >(locationContent, locationCgis));
				locationContent.clear();
				locationCgis.clear();
				break;
			case SERVER:
				addServer(serverContent, locationBlocks);
				locationBlocks.clear();
				serverContent.clear();
				break;
			case NONE:
				throw std::runtime_error("shouldn't happen.");
			}
			continue;
		}
		if (line[line.size() - 1] != ';')
			throw std::runtime_error("unexpected end of line at line ");
		if (blockstack.size() > 0) {
			if (blockstack.top() == CGIBLOCK)
				cgiContent.read(line);
			else
				(blockstack.top() == LOCATION ? locationContent : serverContent) += line + '\n';
		}
	}
	if (this->_server.empty())
		throw std::runtime_error("no server entries found");
}

ConfigParser::~ConfigParser() {}

void ConfigParser::addServer(std::string content, std::vector<std::pair<std::string, std::vector<CGI> > > location)
{
	ServerConfig newServ(content, location);
	_server.push_back(newServ);
}

bool ConfigParser::processServerStart(const std::string& line, Block& waitBrace, std::stack<Block>& blockstack)
{
	if (utils::startswith(line, "server")) {
		if (line == "server {") {
			blockstack.push(SERVER);
		}
		else if (line == "server")
			waitBrace = SERVER;
		else
			return false;
		return true;
	}
	return false;
}

bool ConfigParser::processLocationStart(const std::string& line, Block& waitBrace, std::stack<Block>& blockstack, std::string& locationContent)
{
	if (utils::startswith(line, "location ")) {
		if (utils::endswith(line, "{")) {
			locationContent += utils::smartSubstr(line, "location", "{") + "\n";
			blockstack.push(LOCATION);
		}
		else {
			locationContent += line.substr(std::string("location ").size()) + "\n";
			waitBrace = LOCATION;
		}
		return true;
	}
	return false;
}

bool ConfigParser::processCgiStart(const std::string& line, Block& waitBrace, std::stack<Block>& blockstack, CgiBuilder& cgiContent)
{
	if (utils::startswith(line, "cgi ")) {
		std::string ext;
		if (utils::endswith(line, "{")) {
			ext = utils::smartSubstr(line, "cgi", "{");
			blockstack.push(CGIBLOCK);
		}
		else {
			ext = line.substr(std::string("cgi ").size());
			waitBrace = LOCATION;
		}
		ext = utils::smartSubstr(line, "cgi", "{");
		if (utils::startswith(ext, "."))
			ext = ext.substr(1);
		else
			throw std::runtime_error("extension for CGI must start with '.'");
		cgiContent.extension = ext;
		return true;
	}
	return false;
}

bool ConfigParser::processOpeningBrace(const std::string& line, Block& waitBrace, std::stack<Block>& blockstack)
{
	if (line == "{") {
		if (!waitBrace)
			throw std::runtime_error("Unexpected Brace");
		blockstack.push(waitBrace);
		waitBrace = NONE;
		return true;
	}
	return false;
}

std::vector<ServerConfig> ConfigParser::getServer() { return _server; }
