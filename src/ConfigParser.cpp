#include "ConfigParser.hpp"
#include "utils.hpp"

ConfigParser::ConfigParser(File config)
{
	std::string line;
	std::string serverContent;
	std::string locationContent;
	std::vector<std::string> locationBlocks;
	int inServer = 0;
	int inLocation = 0;
	int waitBrace = 0;

	for (size_t i = 0; i < config.GetSize(); ++i) {
		line = utils::trim(config.getLine(i));
		if (line.empty() || line[0] == '#')
			continue;

		if (processServerStart(line, waitBrace, inServer))
			continue;
		if (processLocationStart(line, waitBrace, inLocation))
			continue;
		if (processOpeningBrace(line, waitBrace, inServer, inLocation))
			continue;
		if (processFullLineServer(line, inServer))
			continue;
		if (processFullLineLocation(line, inLocation))
			continue;

		if (waitBrace || line == "{")
			throw std::runtime_error("Invalid config file (brace issue)");

		if (line == "}") {
			if (inLocation) {
				inLocation--;
				locationBlocks.push_back(locationContent);
				locationContent.clear();
			} else if (inServer) {
				inServer--;
				AddServer(serverContent, locationBlocks);
				locationBlocks.clear();
				serverContent.clear();
			} else {
				throw std::runtime_error("Unexpected closing brace");
			}
			continue;
		}

		(inLocation > 0 ? locationContent : serverContent) += line + '\n';
	}
}

ConfigParser::~ConfigParser() {}

void ConfigParser::AddServer(std::string content, std::vector<std::string> location)
{
	ServerConfig newServ(content, location);
	_server.push_back(newServ);
}

bool ConfigParser::processServerStart(const std::string& line, int& waitBrace, int inServer)
{
	if (line == "server") {
		if (inServer >= 1)
			throw std::runtime_error("server blocks in a server blocks are not allowed");
		waitBrace = 1;
		return true;
	}
	return false;
}

bool ConfigParser::processLocationStart(const std::string& line, int& waitBrace, int inLocation)
{
	if (line.find("location") != std::string::npos && line.find("{") == std::string::npos) {
		if (inLocation >= 1)
			throw std::runtime_error("location blocks in a location blocks are not allowed");
		waitBrace = 2;
		return true;
	}
	return false;
}

bool ConfigParser::processOpeningBrace(const std::string& line, int& waitBrace, int& inServer, int& inLocation)
{
	if (line == "{") {
		if (waitBrace == 1)
			inServer++;
		else if (waitBrace == 2)
			inLocation++;
		else
			throw std::runtime_error("Unexpected Brace");
		waitBrace = 0;
		return true;
	}
	return false;
}

bool ConfigParser::processFullLineServer(const std::string& line, int& inServer)
{
	if (line == "server {") {
		inServer++;
		return true;
	}
	return false;
}

bool ConfigParser::processFullLineLocation(const std::string& line, int& inLocation)
{
	if (line.find("location") != std::string::npos && line.find("{") != std::string::npos) {
		inLocation++;
		return true;
	}
	return false;
}

std::vector<ServerConfig> ConfigParser::getServer() { return _server; }