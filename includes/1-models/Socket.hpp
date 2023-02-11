#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <iostream>

class Socket {
	public:
		// -Constructors
		Socket(void);
		Socket(Socket const &rhs);

		// -Destructor
		~Socket(void);

		// -Operators
		Socket &operator=(Socket const &rhs);

		// -Getters

		// -Setters

		// -Methods

	private:

	protected:

};

// -Functions
std::ostream &operator<<(std::ostream &out, Socket const &in);

#endif
