#include "../../includes/1-models/Server.hpp"
// -Constructors
Server::Server(void) {
	std::cout << "Server default constructor called\n";
	return ;
}

Server::Server(Server const &rhs) {
	std::cout << "Server copy constructor called\n";
	*this = rhs;
	return ;
}

// -Destructor
Server::~Server(void) {
	std::cout << "Server default destructor called\n";
	return ;
}

// -Operators
Server &Server::operator=(Server const &rhs) {
	if (this != &rhs) {
		std::cout << "Server copy assignment operator called\n";
	}
	return (*this);
}

// -Getters
// -Setters
// -Methods
// -Functions
std::ostream &operator<<(std::ostream &out, Server const &in) {
  (void)in;
  return (out);
}
