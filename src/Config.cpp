#include "Config.hpp"

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
	else if (key == "client_max_body_size")
		this->_clientMaxBodySize = atoi(value.c_str());
	else if (key == "error_page")
	{
		std::string pLine = utils::smartSubstr(line, "error_pages", "/");
		this->_errorPages.insert(std::pair<int,std::string>(atoi(pLine.c_str()), utils::smartSubstr(line, pLine, ";")));
	}
	else
		return (0);
	return (1);
}
	
bool Config::getAutoIndex(){return this->_autoIndex;}
std::vector<std::string> Config::getCgiExtension(){return this->_cgiExtension;}
bool Config::getUploadEnabled(){return this->_uploadEnabled;}
std::vector<std::string> Config::getUploadPath(){return this->_uploadPath;}
std::string Config::getRoot() const {return this->_root;}
std::vector<std::string> Config::getIndex() const {return this->_index;}