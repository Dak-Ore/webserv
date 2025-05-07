#ifndef FILE_HPP
# define FILE_HPP

# include <iostream>
# include <vector>
# include <cstring>
# include <exception>
# include <fstream>

/**
 * Represents the content of a file.
 */
class File
{
public:
    // Constructor
    File(std::string filename);
    
    // Destructor
    ~File();

    /**
     * Get a line of the file from its index, ended by '\n'.
     */
	std::string getLine(size_t nb);

	/**
	 * Get all the content of the files. The lines are ended by '\n'.
	 */
	std::string getContent();

	/**
	 * Get the number of lines of the file.
	 */
	size_t		getLineNumber();

private:
	std::string _name;
	std::vector<std::string> _content; 
};

#endif
