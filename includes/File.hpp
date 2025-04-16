#ifndef FILE_HPP
# define FILE_HPP

# include <iostream>
# include <vector>
# include <cstring>
# include <exception>
# include <fstream>

class File
{
public:
    // Default Constructor
    File(std::string name);
    
    // Destructor
    ~File();

	std::string getLine(size_t nb);
	std::string getContent();
	size_t		GetSize();
private:
	std::string _name;
	std::vector<std::string> _content; 
};

std::string	smartSubstr(std::string line, std::string start, std::string end);

#endif
