#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include <iostream>
#include <vector>
#include "Server.hpp"

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
		void	addServer();
	private:
		std::vector<Server*>	_serverList;
	protected:

};

// -Functions
std::ostream &operator<<(std::ostream &out, WebServer const &in);

#endif
