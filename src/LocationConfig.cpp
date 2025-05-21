#include "LocationConfig.hpp"
#include "utils.hpp"
#include <vector>
#include "ServerConfig.hpp"

LocationConfig::LocationConfig(std::string content, std::vector<CGI> cgis, const ServerConfig &father): Config(), _cgis(cgis)
{
  	std::istringstream stream(content);
	std::string		line;
	
	this->_hasRedirection = false;

	stream >> this->_path;
	std::getline(stream, line);
	while (std::getline(stream, line) && this->_hasRedirection == false)
		this->findElement(line);
	this->checkConfig();
	heritFromFather(father);
}

void LocationConfig::heritFromFather(const ServerConfig &father) {
    if (!this->flags.hasRoot)
        this->_root = father.getRoot();

    if (!this->flags.hasIndex)
		this->_index = father.getIndex();

    if (!this->flags.hasAutoIndex)
        this->_autoIndex = father.getAutoIndex();

    if (!this->flags.hasClientMaxBodySize)
        this->_clientMaxBodySize = father.getClientMaxBodySize();

    if (!this->flags.hasUploadEnabled)
        this->_uploadEnabled = father.getUploadEnabled();

    if (!this->flags.hasUploadPath)
        this->_uploadPath = father.getUploadPath();

    if (!this->flags.hasAllowedMethods)
		this->_allowedMethods = father.getAllowedMethods();

	if (this->flags.hasErrorPages)
		this->_errorPages = father.getErrorPages();
}


void	LocationConfig::checkConfig()
{
	if (!utils::isValidRegex(this->_path, "^/[A-Za-z0-9_./*-]*$"))
		throw std::runtime_error("Invalid path " + this->_path + " in location config");
	if (this->_hasRedirection)
	{
		if (this->_redirection.first < 100 || this->_redirection.first > 599)
			throw std::runtime_error("invalid redirection code return code in location block");
		if (!utils::isValidRegex(this->_redirection.second, "^(https?://[^\\s]+|/[^\\s]*)$"))
			throw std::runtime_error("invalid return redirection " + this->_redirection.second + " in location block");
	}	
}

void	LocationConfig::findElement(std::string line)
{
	std::string key = utils::extractKey(line);
	std::string value = utils::smartSubstr(line, key, ";");

	if (parseVar(key, value, line))
		return ;
	else if (key == "return")
	{
		this->_hasRedirection = true;
		std::string word = utils::extractKey(value);
		this->_redirection.first = atoi(word.c_str());
		this->_redirection.second = utils::smartSubstr(value, word, ";");
	}
	else
		std::cerr << "! Warning ! In configuration file: parameter '" << key << "' is unknown. Ignored." << std::endl;
}

void		LocationConfig::print()
{
	std::cout << std::endl << "Location" << this->_path << std::endl;
	std::cout << this->_root << std::endl;
	std::cout << "Upload enabled: " << this->_uploadEnabled << " Autoindex: " << this->_autoIndex << " Has redirection: " << this->_hasRedirection << std::endl;
	std::cout << "Redirection : " << this->_redirection.first << this->_redirection.second << std::endl;
	std::cout << std::endl << "Allowed methods : ";
	for (std::vector<std::string>::iterator it = this->_allowedMethods.begin(); it != this->_allowedMethods.end(); it++)
		std::cout << *it << " - ";
	std::cout << std::endl << "Index : ";
	for (std::vector<std::string>::iterator it = this->_index.begin(); it != this->_index.end(); it++)
		std::cout << *it << " - ";
	std::cout << std::endl << "Upload path : ";
	for (std::vector<std::string>::iterator it = this->_uploadPath.begin(); it != this->_uploadPath.end(); it++)
		std::cout << *it << " - ";
	std::cout << std::endl;
}

// Destructor
LocationConfig::~LocationConfig() {
}

std::string LocationConfig::getRelativePath(const std::string& path) const
{
	std::string locPath = utils::addTrailingSlash(this->getPath());
	std::string normPath = utils::addTrailingSlash(path);

	// std::cout << "   - LOCATION: " << locPath << std::endl;
	if (normPath.compare(0, locPath.length(), locPath) == 0)
		return utils::removeTrailingSlash(path.substr(locPath.length() - 1));
	return ("");
}

CGI const* LocationConfig::getCgi(std::string const& filename) const
{
	for (std::vector<CGI>::const_iterator it = this->_cgis.begin(); it != this->_cgis.end(); it++)
		if (it->fileForMe(filename))
			return &*it;  // this is art
	return NULL;
}

//getters
std::string LocationConfig::getPath() const{return this->_path;}
bool LocationConfig::getHasRedirection() const{return this->_hasRedirection;}
std::pair<int, std::string> LocationConfig::getRedirection() const{return this->_redirection;}
