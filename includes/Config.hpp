#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <iostream>
# include <utils.hpp>
# include <vector>
# include <map>
# include <sstream>
# include <cmath>
# include <cstdlib>

/*
    Value that can be in LocationConfig and ServerConfig
*/
class Config
{
protected:
    /*
        Link to the directory that contain the pages.
        A server or a location can have different root, default is www
    */
    std::string _root;
    /*
    List of index, default index is index.html
    */
   std::vector<std::string> _index;
    /*
       True : 
       
       False :
           Check if at least an index is valid 
   */
   bool _autoIndex;
   /*
        Size of request body, if not specified there is no limit (or maybe linux limits)
        Value is digits and can be followed by k/m/g (not case sensitive)
    */
    size_t _clientMaxBodySize;
    /*
        Key   : int error ex 404
        Value : string path to error page
    */
    std::map<int, std::string> _errorPages;
    /*
        Default : false
    */
    bool _uploadEnabled;
    /*
        Where we upload the file, need to check if the path is correct and is a directory
    */
    std::vector<std::string> _uploadPath;
    /*
        Method allowed in the server / location, all allowed.
        We handle only GET / POST / DELETE.
    */
    std::vector<std::string> _allowedMethods;
public:
    // default constructor
	Config();
    // default destructor
	~Config();
    
    // GETTERS
	const std::string  &getRoot() const;
    const std::vector<std::string>     	&getIndex() const;
	size_t                              getClientMaxBodySize() const;
    const std::map<int, std::string>&   getErrorPages() const;
	bool                                getAutoIndex() const;
    bool                                getUploadEnabled() const;
    const std::vector<std::string>      &getUploadPath()const;
	const std::vector<std::string>      &getAllowedMethods()const;


    struct Flags {
        bool hasRoot;
        bool hasIndex;
        bool hasAutoIndex;
        bool hasClientMaxBodySize;
        bool hasUploadEnabled;
        bool hasUploadPath;
        bool hasAllowedMethods;
        bool hasErrorPages;
    } flags;

    /*
        Check is parsed var is valid.
        current check : 
            - root : check is valid character
            - allow_methods : Check if element of the list is GET / POST / DELETE
    */
	void checkVar();
    /**
    	* @brief Handle the parsing of a line for children.
        * @param key key of a parameter on the config file.
        * @param value value(s) of a parameter on the config file.
        * @param line complete line to handle certain case.
    */
	int	parseVar(std::string key, std::string value, std::string line);
	virtual void	findElement(std::string line) = 0;

	virtual std::string getRelativePath(const std::string& path) const = 0;
};

#endif
