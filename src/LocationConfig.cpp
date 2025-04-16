#include "LocationConfig.hpp"

// Default Constructor
LocationConfig::LocationConfig(std::string content) {
  	std::istringstream stream(content);
	std::string		line;
	
	this->_hasRedirection = false;
	if (!(stream >> this->_path))
		throw std::runtime_error("invalid location");
	stream >> this->_path;
	std::getline(stream, line);
	while (std::getline(stream, line) && this->_hasRedirection == false)
		this->findElement(line);
}

void	LocationConfig::findElement(std::string line)
{
	std::string	elements[] = {"allowed_methods", "autoindex", "root", "return", "upload_path", "upload_enabled", "cgi_extension", "index"};
	int i = 0;
	size_t pos;
	std::string elemLine;
	for (i = 0; i < 8; i++)
	{
		pos = line.find(elements[i]);
		if (pos != std::string::npos)
			{
				if (pos != 0)
					throw std::runtime_error("invalid line in config file");
				break;
			}
		}
	if (i < 8)
		elemLine = smartSubstr(line, elements[i], ";");
	switch (i)
	{
		case 0:
			this->splitPush(elemLine, 0);
			break;
		case 1:
			if (elemLine == "on")
				this->_autoIndex = true;
			else
				this->_autoIndex =false;
			break;
		case 2:
			this->_root = elemLine;
			break;
		case 3:
		{
			std::istringstream	pStream(elemLine);
			std::string			word;
			this->_hasRedirection = true;
			pStream >> word;
			this->_redirection.first = atoi(word.c_str());
			pStream >> this->_redirection.second;
			break;
		}
		case 4:
			this->_uploadPath.push_back(elemLine);
			break;
		case 5:{
			this->_uploadEnabled = true;
			break;
		}
		case 6:
			this->_cgiExtension.push_back(elemLine);
			break;
		case 7:
			this->splitPush(elemLine, 1);
			break;
		default:
			throw std::runtime_error("invalid line in config file");
	}
}

void	LocationConfig::splitPush(std::string line, int kind)
{
	std::istringstream	stream(line);
	std::string			word;

	while (stream >> word)
	{
		if (kind == 0)
			this->_allowedMethods.push_back(word);
		else
			this->_index.push_back(word);
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

//getters
std::string LocationConfig::getPath(){return this->_path;}
std::vector<std::string> LocationConfig::getAllowedMethods(){return this->_allowedMethods;}
std::string LocationConfig::getRoot(){return this->_root;}
std::vector<std::string> LocationConfig::getIndex(){return this->_index;}
bool LocationConfig::getAutoIndex(){return this->_autoIndex;}
bool LocationConfig::getHasRedirection(){return this->_hasRedirection;}
std::pair<int, std::string> LocationConfig::getRedirection(){return this->_redirection;}
std::vector<std::string> LocationConfig::getCgiExtension(){return this->_cgiExtension;}
bool LocationConfig::getUploadEnabled(){return this->_uploadEnabled;}
std::vector<std::string> LocationConfig::getUploadPath(){return this->_uploadPath;}