#include "WebServer.hpp"

// -Constructors
WebServer::WebServer(void) {
	this->total = 0;
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
void WebServer::addServer() {
	this->total++;
	Server *add = new Server(total);
	this->_serverList.push_back(add);
}

Server &WebServer::getLastServer() {
	return (*this->_serverList.back());
}

// -Functions
std::ostream &operator<<(std::ostream &out, WebServer const &in) {
	(void)in;
	return (out);
}
