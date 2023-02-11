#include "Socket.hpp"

// -Constructors
Socket::Socket(void) {
	std::cout << "Socket default constructor called\n";
	return ;
}

Socket::Socket(Socket const &rhs) {
	std::cout << "Socket copy constructor called\n";
	*this = rhs;
	return ;
}

// -Destructor
Socket::~Socket(void) {
	std::cout << "Socket default destructor called\n";
	return ;
}

// -Operators
Socket &Socket::operator=(Socket const &rhs) {
	if (this != &rhs) {
		std::cout << "Socket copy assignment operator called\n";
	}
	return (*this);
}

// -Getters
// -Setters
// -Methods
// -Functions
std::ostream &operator<<(std::ostream &out, Socket const &in) {
	(void)in;
	return (out);
}
