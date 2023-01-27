#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include <iostream>

class ServerConfig {
	public:
		// -Constructors
		ServerConfig(void);
		ServerConfig(ServerConfig const &rhs);

		// -Destructor
		~ServerConfig(void);

		// -Operators
		ServerConfig &operator=(ServerConfig const &rhs);

		// -Getters

		// -Setters

		// -Methods

	private:

	protected:

};

// -Functions
std::ostream &operator<<(std::ostream &out, ServerConfig const &in);

#endif
