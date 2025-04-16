#include "LocationConfig.hpp"

// Default Constructor
LocationConfig::LocationConfig(std::string content) {
  	std::istringstream stream(content);
	std::string		line;
	
	this->_hasRedirection = false;
	if (!(stream >> this->_path))
		throw std::runtime_error("invalid location");
	stream >> this->_path;
	std::cout << this->_path << std::endl;
	std::getline(stream, line);
	while (std::getline(stream, line) && this->_hasRedirection == false)
		this->findElement(line);
}

void	LocationConfig::findElement(std::string line)
{
	std::string	elements[] = {"allowed_methods", "autoindex", "root", "return", "upload_path", "upload_enabled", "cgi_extension", "index"};
	int i = 0;
	size_t pos;
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

	switch (i)
	{
		case 0:
			this->splitPush(smartSubstr(line, elements[i], ";"), 0);
			break;
		case 1:
			if (smartSubstr(line, "autoindex", ";") == "on")
				this->_autoIndex = true;
			else
				this->_autoIndex =false;
			break;
		case 2:
			this->_root = smartSubstr(line, "root", ";");
			break;
		case 3:
		{
			std::string pLine = smartSubstr(line, "return", ";");
			std::istringstream	pStream(pLine);
			std::string			word;
			this->_hasRedirection = true;
			pStream >> word;
			this->_redirection.first = atoi(word.c_str());
			pStream >> this->_redirection.second;
			break;
		}
		case 4:
			this->_uploadPath.push_back(smartSubstr(line, elements[i], ";"));
			break;
		case 5:{
			this->_uploadEnabled = true;
			break;
		}
		case 6:
			this->_cgiExtension.push_back(smartSubstr(line, elements[i], ";"));
			break;
		case 7:
			this->splitPush(smartSubstr(line, elements[i], ";"), 1);
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


// Destructor
LocationConfig::~LocationConfig() {
}
