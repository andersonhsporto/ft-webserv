#ifndef SERVERLOCATION_HPP
#define SERVERLOCATION_HPP

#include <iostream>

class ServerLocation {
	public:
		// -Constructors
		ServerLocation(void);
		ServerLocation(ServerLocation const &rhs);

		// -Destructor
		~ServerLocation(void);

		// -Operators
		ServerLocation &operator=(ServerLocation const &rhs);

		// -Getters

		// -Setters

		// -Methods

	private:

	protected:

};

// -Functions
std::ostream &operator<<(std::ostream &out, ServerLocation const &in);

#endif
