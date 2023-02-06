#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>

class Request {
	public:
		// -Constructors
		Request(void);
		Request(Request const &rhs);

		// -Destructor
		~Request(void);

		// -Operators
		Request &operator=(Request const &rhs);

		// -Getters

		// -Setters

		// -Methods

	private:

	protected:

};

// -Functions
std::ostream &operator<<(std::ostream &out, Request const &in);

#endif
