#include "Config.hpp"

Config::Config()
{
	this->_root = "www";
	this->_clientMaxBodySize = pow(1024, 2);
	this->_uploadEnabled = false;
	this->_autoIndex = false;
	this->_index.push_back("index.html");

	this->flags.hasAllowedMethods = false;
	this->flags.hasAutoIndex = false;
	this->flags.hasClientMaxBodySize = false;
	this->flags.hasErrorPages = false;
	this->flags.hasIndex = false;
	this->flags.hasRoot = false;
	this->flags.hasUploadEnabled = false;
	this->flags.hasUploadPath = false;
}


Config::~Config()
{
}

int	Config::parseVar(std::string key, std::string value, std::string line)
{
	if (key == "autoindex")
	{
		this->flags.hasAutoIndex = true;
		this->_autoIndex = value == "on";
	}
	else if (key == "root")
	{
		this->flags.hasRoot = true;
		this->_root = value;
	}
	else if (key == "upload_path")
	{
		this->flags.hasUploadPath = true;
		this->_uploadPath.push_back(value);
	}
	else if (key == "upload_enabled")
	{
		this->flags.hasUploadEnabled = true;
		this->_uploadEnabled = true;
	}
	else if (key == "index")
	{
		this->flags.hasIndex = true;
		utils::ft_split(value, &this->_index);
	}
	else if (key == "allow_methods")
	{
		this->flags.hasAllowedMethods = true;
		utils::ft_split(value, &this->_allowedMethods);
	}
	else if (key == "client_max_body_size")
	{
		this->flags.hasClientMaxBodySize = true;
		if (utils::isValidRegex(value, "^0-9$"))
			this->_clientMaxBodySize = atoi(value.c_str());
		else if (!utils::isValidRegex(value, "^0-9([kKmMgG]?)$"))
		{
			char o = value[value.size() - 1];
			if (o == 'k' || o == 'K')
				this->_clientMaxBodySize = atoi(value.c_str()) * 1024;
			if (o == 'm' || o == 'M')
				this->_clientMaxBodySize = atoi(value.c_str()) * pow(1024, 2);
			if (o == 'g' || o == 'G')
				this->_clientMaxBodySize = atoi(value.c_str()) * pow(1024, 3);
		}
	}
	else if (key == "error_page")
	{
		this->flags.hasErrorPages = true;
		std::string pLine = utils::extractElem(line, 2);
		this->_errorPages.insert(std::pair<int,std::string>(atoi(pLine.c_str()), utils::smartSubstr(line, pLine, ";")));
	}
	else
		return (0);
	return (1);
}

void	Config::checkVar()
{
	if (!this->_root.empty() && !utils::isValidRegex(this->_root, "^[A-Za-z0-9\\/_.-]*$"))
		throw std::runtime_error("Invalid root path : " + this->_root + " in configuration file");
	for (std::vector<std::string>::iterator it = this->_allowedMethods.begin(); it != this->_allowedMethods.end(); it++)
		if (!utils::isValidRegex(*it, "^(GET|POST|DELETE)$"))
			throw std::runtime_error("invalid argument of allow_methods " + *it);
}

const std::map<int, std::string>& Config::getErrorPages() const {return this->_errorPages;}
bool Config::getAutoIndex() const {return this->_autoIndex;}
bool Config::getUploadEnabled() const {return this->_uploadEnabled;}
const std::vector<std::string> &Config::getUploadPath() const {return this->_uploadPath;}
const std::string &Config::getRoot() const {return this->_root;}
const std::vector<std::string> &Config::getIndex() const {return this->_index;}
const std::vector<std::string> &Config::getAllowedMethods() const{return this->_allowedMethods;}
size_t Config::getClientMaxBodySize() const {return this->_clientMaxBodySize;}

std::string Config::findIndex(const std::string& path, std::string* filename)
{
	const std::string& root = this->getRoot();
	const std::vector<std::string>& indexList = this->getIndex();
	std::string directory = utils::addTrailingSlash(utils::joinPath(root, path));

	for (size_t i = 0; i < indexList.size(); ++i)
	{
		std::string file_path = directory + indexList[i];
		if (utils::fileExists(file_path)) {
			if (filename)
				*filename = indexList[i];
			return (file_path);
		}
	}
	return std::string();
}