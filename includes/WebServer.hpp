#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include <iostream>
#include <cstdlib>
#include <vector>
#include "Server.hpp"
#include "ParserConfig.hpp"
#include "Poll.hpp"
#include "Utils.hpp"

class WebServer {
	public:
		// -Constructors
		WebServer(void);
		WebServer(WebServer const &rhs);

		// -Destructor
		~WebServer();

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
		std::string				_rawRequest;

		// -Methods
		size_t	_sizeBody(const std::string &request);
		size_t _findContentLenght(const std::string &request);
};

// -Functions
std::ostream &operator<<(std::ostream &out, WebServer const &in);

#endif
