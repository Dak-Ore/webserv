#ifndef CONFIGPARSER_HPP
# define CONFIGPARSER_HPP

# include <iostream>
# include "ServerConfig.hpp"
# include "File.hpp"
# include "CGI.hpp"
# include <stack>
# include <string>
# include <cctype>

/**
 * Creates ServerConfig-s from a config file.
 */
class ConfigParser
{
public:
    // Constructor
    ConfigParser(File config);
    // Destructor
    ~ConfigParser();
    /**
     * Get the ServerConfig-s.
     */
	std::vector<ServerConfig>	getServer();
private:
	enum Block { NONE, SERVER, LOCATION, CGIBLOCK };
	std::vector<ServerConfig>	_server;

	struct CgiBuilder
	{
		CgiBuilder();
		CGI toCgi();
		void clear();
		void read(std::string line);

		std::string extension;
private:

		std::string _binary;
		bool _pass_script_through_argv;
		bool _pass_script_through_argv_set;
	};

	std::string nameOfBlock(Block block);
	Block blockFromName(std::string name);
	void addServer(std::string content, std::vector<std::pair<std::string, std::vector<CGI> > > location);
	bool processServerStart(const std::string& line, Block& waitBrace, std::stack<Block>& blockstack);
	bool processLocationStart(const std::string& line, Block& waitBrace, std::stack<Block>& blockstack, std::string& locationContent);
	bool processOpeningBrace(const std::string& line, Block& waitBrace, std::stack<Block>& blockstack);
	bool processCgiStart(const std::string& line, Block& waitBrace, std::stack<Block>& blockstack, CgiBuilder& cgiContent);

};

#endif
