#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <vector>
#include <map>
#include "ServerLocation.hpp"

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

    // listen  127.0.0.1:4242
    int port;
    int host;

    // server_name www.localhost localhost;
    std::vector<std::string> serverName;

    // TODO
    std::string root;

    // index index.html;
    std::vector<std::string> index;

    // error_page 404 /404.html;
    // std::pair<int, std::string> errorPage; ??
    std::map<int, std::string> errorPages;

    // location / {
    std::vector<ServerLocation> locations;



	protected:

};

// -Functions
std::ostream &operator<<(std::ostream &out, Server const &in);

#endif
