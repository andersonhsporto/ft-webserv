#include "../../includes/ServerLocation.hpp"

// -Constructors
ServerLocation::ServerLocation(void) {
	std::cout << "ServerLocation default constructor called\n";
	return ;
}

ServerLocation::ServerLocation(ServerLocation const &rhs) {
	std::cout << "ServerLocation copy constructor called\n";
	*this = rhs;
	return ;
}

// -Destructor
ServerLocation::~ServerLocation(void) {
	std::cout << "ServerLocation default destructor called\n";
	return ;
}

// -Operators
ServerLocation &ServerLocation::operator=(ServerLocation const &rhs) {
	if (this != &rhs) {
		std::cout << "ServerLocation copy assignment operator called\n";
	}
	return (*this);
}

// -Getters
// -Setters
// -Methods
// -Functions
std::ostream &operator<<(std::ostream &out, ServerLocation const &in) {
	return (out);
}
