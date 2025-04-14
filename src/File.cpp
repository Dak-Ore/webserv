#include "File.hpp"

// Default Constructor
File::File(std::string name) : _name(name)
{
	std::ifstream	fileRead(_name.c_str(), std::ios::in);
	std::string		line;
	if (fileRead)
	{
		while (fileRead.good())
		{
			std::getline(fileRead, line);
			if (!line.empty())
				this->_content.push_back(line);
		}
		fileRead.close();
	}
	else
		throw std::runtime_error("Error: when trying to open a file.");
}

std::string File::getLine(size_t nb)
{
	if (nb > this->_content.size())
		throw std::runtime_error("Error trying to get line of a file that doesn't exist.");
	else
		return (this->_content[nb] + '\n');
}

std::string File::getContent()
{
	std::string content;
	for (std::vector<std::string>::iterator it = this->_content.begin(); it != this->_content.end(); it ++)
		content.append(*it + '\n');
	return content;
}

size_t File::GetSize()
{
	return this->_content.size();
}

// Destructor
File::~File() {

}


