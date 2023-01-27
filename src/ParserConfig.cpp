#include "ParserConfig.hpp"

// -Constructors
ParserConfig::ParserConfig(void) {
	std::cout << "ParserConfig default constructor called\n";
	return ;
}

ParserConfig::ParserConfig(ParserConfig const &rhs) {
	std::cout << "ParserConfig copy constructor called\n";
	*this = rhs;
	return ;
}

// -Destructor
ParserConfig::~ParserConfig(void) {
	std::cout << "ParserConfig default destructor called\n";
	return ;
}

// -Operators
ParserConfig &ParserConfig::operator=(ParserConfig const &rhs) {
	if (this != &rhs) {
		std::cout << "ParserConfig copy assignment operator called\n";
	}
	return (*this);
}

// -Getters
// -Setters
// -Methods
// -Functions
std::ostream &operator<<(std::ostream &out, ParserConfig const &in) {
	return (out);
}
