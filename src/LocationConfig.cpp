#include "LocationConfig.hpp"
#include "utils.hpp"

LocationConfig::LocationConfig(std::string content): Config()
{
  	std::istringstream stream(content);
	std::string		line;
	
	this->_hasRedirection = false;
	this->_uploadEnabled = false;
	stream >> this->_path;
	std::getline(stream, line);
	while (std::getline(stream, line) && this->_hasRedirection == false)
		this->findElement(line);
	this->checkConfig();
}

void	LocationConfig::checkConfig()
{
	this->checkVar();
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
}

void		LocationConfig::print()
{
	std::cout << std::endl << "Location" << this->_path << std::endl;
	std::cout << this->_root << std::endl;
	std::cout << "Upload enabled: " << this->_uploadEnabled << " Autoindex: " << this->_autoIndex << " Has redirection: " << this->_hasRedirection << std::endl;
	std::cout << "Redirection : " << this->_redirection.first << this->_redirection.second << std::endl;
	std::cout << "CGI Extension : ";
	for (std::vector<std::string>::iterator it = this->_cgiExtension.begin(); it != this->_cgiExtension.end(); it++)
		std::cout << *it << " - ";
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

//getters
std::string LocationConfig::getPath() const{return this->_path;}
bool LocationConfig::getHasRedirection() const{return this->_hasRedirection;}
std::pair<int, std::string> LocationConfig::getRedirection() const{return this->_redirection;}