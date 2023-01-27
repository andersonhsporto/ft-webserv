#include "ServerConfig.hpp"

// -Constructors
ServerConfig::ServerConfig(void) {
	std::cout << "ServerConfig default constructor called\n";
	return ;
}

ServerConfig::ServerConfig(ServerConfig const &rhs) {
	std::cout << "ServerConfig copy constructor called\n";
	*this = rhs;
	return ;
}

// -Destructor
ServerConfig::~ServerConfig(void) {
	std::cout << "ServerConfig default destructor called\n";
	return ;
}

// -Operators
ServerConfig &ServerConfig::operator=(ServerConfig const &rhs) {
	if (this != &rhs) {
		std::cout << "ServerConfig copy assignment operator called\n";
	}
	return (*this);
}

// -Getters
// -Setters
// -Methods
// -Functions
std::ostream &operator<<(std::ostream &out, ServerConfig const &in) {
	return (out);
}
