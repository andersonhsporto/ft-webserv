#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include <iostream>
#include <vector>
#include "Server.hpp"
#include "ParserConfig.hpp"
#include "Poll.hpp"

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
		void	run(const std::string &FilePath);
		void	addServer();
		Server	&getLastServer();
		void	finish(void);

	private:
		ParserConfig			_parser;
		std::vector<Server*>	_serverList;
		Poll					_poller;
};

// -Functions
std::ostream &operator<<(std::ostream &out, WebServer const &in);

#endif
