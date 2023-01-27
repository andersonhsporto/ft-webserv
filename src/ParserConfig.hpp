#ifndef PARSERCONFIG_HPP
#define PARSERCONFIG_HPP

#include <iostream>

class ParserConfig {
	public:
		// -Constructors
		ParserConfig(void);
		ParserConfig(ParserConfig const &rhs);

		// -Destructor
		~ParserConfig(void);

		// -Operators
		ParserConfig &operator=(ParserConfig const &rhs);

		// -Getters

		// -Setters

		// -Methods

	private:

	protected:

};

// -Functions
std::ostream &operator<<(std::ostream &out, ParserConfig const &in);

#endif
