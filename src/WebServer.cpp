#include "WebServer.hpp"

// -Constructors
WebServer::WebServer(void) {
	std::cout << "WebServer default constructor called\n";
	return ;
}

WebServer::WebServer(WebServer const &rhs) {
	std::cout << "WebServer copy constructor called\n";
	*this = rhs;
	return ;
}

// -Destructor
WebServer::~WebServer(void) {
	std::cout << "WebServer default destructor called\n";
	return ;
}

// -Operators
WebServer &WebServer::operator=(WebServer const &rhs) {
	if (this != &rhs) {
		std::cout << "WebServer copy assignment operator called\n";
	}
	return (*this);
}

// -Getters
// -Setters
// -Methods
// -Functions
std::ostream &operator<<(std::ostream &out, WebServer const &in) {
	return (out);
}
