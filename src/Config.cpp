#include "Config.hpp"

Config::Config()
{
	this->_root = "www";
	this->_clientMaxBodySize = pow(1024, 2);
	this->_uploadEnabled = false;
	this->_autoIndex = false;
	this->_index.push_back("index.html");
}

Config::~Config()
{
}

int	Config::parseVar(std::string key, std::string value, std::string line)
{
	if (key == "autoindex")
		this->_autoIndex = value == "on";
	else if (key == "root")
		this->_root = value;
	else if (key == "upload_path")
		this->_uploadPath.push_back(value);
	else if (key == "upload_enabled")
		this->_uploadEnabled = true;
	else if (key == "cgi_extension")
		utils::ft_split(value, &this->_cgiExtension);
	else if (key == "index")
		utils::ft_split(value, &this->_index);
	else if (key == "allow_methods")
		utils::ft_split(value, &this->_allowedMethods);
	else if (key == "client_max_body_size")
	{
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
		std::string pLine = utils::smartSubstr(line, "error_pages", "/");
		this->_errorPages.insert(std::pair<int,std::string>(atoi(pLine.c_str()), utils::smartSubstr(line, pLine, ";")));
	}
	else
		return (0);
	return (1);
}

void	Config::checkVar()
{
	if (!this->_root.empty() && !utils::isValidRegex(this->_root, "^\\/[A-Za-z0-9\\/_.-]*$"))
		throw std::runtime_error("Invalid root path : " + this->_root + " in configuration file");
	for (std::vector<std::string>::iterator it = this->_allowedMethods.begin(); it != this->_allowedMethods.end(); it++)
		if (!utils::isValidRegex(*it, "^(GET|POST|DELETE)$"))
			throw std::runtime_error("invalid argument of allow_methods " + *it);
}

bool Config::getAutoIndex() const {return this->_autoIndex;}
std::vector<std::string> Config::getCgiExtension() const {return this->_cgiExtension;}
bool Config::getUploadEnabled() const {return this->_uploadEnabled;}
std::vector<std::string> Config::getUploadPath() const {return this->_uploadPath;}
std::string Config::getRoot() const {return this->_root;}
std::vector<std::string> Config::getIndex() const {return this->_index;}
std::vector<std::string> Config::getAllowedMethods() const{return this->_allowedMethods;}