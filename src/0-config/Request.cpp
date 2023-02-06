#include "Request.hpp"

// -Constructors
Request::Request(void) {
	std::cout << "Request default constructor called\n";
	return ;
}

Request::Request(Request const &rhs) {
	std::cout << "Request copy constructor called\n";
	*this = rhs;
	return ;
}

// -Destructor
Request::~Request(void) {
	std::cout << "Request default destructor called\n";
	return ;
}

// -Operators
Request &Request::operator=(Request const &rhs) {
	if (this != &rhs) {
		std::cout << "Request copy assignment operator called\n";
	}
	return (*this);
}

// -Getters
// -Setters
// -Methods
// -Functions
std::ostream &operator<<(std::ostream &out, Request const &in) {
	return (out);
}
