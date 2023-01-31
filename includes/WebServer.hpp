#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include <iostream>
#include <vector>
#include "Server.hpp"
#include "ParserConfig.hpp"

class WebServer {
	public:
		// -Constructors
		WebServer(void);
		WebServer(WebServer const &rhs);

		// -Destructor
		~WebServer(void);

		// -Operators
		WebServer &operator=(WebServer const &rhs);

		// -Getters

		// -Setters

		// -Methods
		void	parse(const std::string &FilePath);
		void	addServer();
		Server	&getLastServer();
	private:
		ParserConfig			_parser;
		std::vector<Server*>	_serverList;
		int		total; // debug
	protected:

};

// -Functions
std::ostream &operator<<(std::ostream &out, WebServer const &in);

#endif
