#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>

class Server {
	public:
		// -Constructors
		Server(void);
		Server(Server const &rhs);

		// -Destructor
		~Server(void);

		// -Operators
		Server &operator=(Server const &rhs);

		// -Getters

		// -Setters

		// -Methods

	private:

	protected:

};

// -Functions
std::ostream &operator<<(std::ostream &out, Server const &in);

#endif
