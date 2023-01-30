#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <vector>
#include <map>
#include <arpa/inet.h>
#include "ServerLocation.hpp"

class Server {
	public:
		// -Constructors
		Server(void);
		Server(int numb);
		Server(Server const &rhs);

		// -Destructor
		~Server(void);

		// -Operators
		Server &operator=(Server const &rhs);

		// -Getters
		const int							&getMaxbodysize(void) const;
		const int							&getTimeout(void) const;
		const std::map<int,std::string>		&getErrorpages(void) const;
		const std::vector<ServerLocation>	&getLocations(void) const;
		const std::vector<std::string>		&getServername(void) const;
		const std::vector<std::string>		&getIndex(void) const;
		const std::string					&getRoot(void) const;
		const int							&getPort(void) const;
		const in_addr_t						&getHost(void) const;

		// -Setters
		void	setMaxbodysize(int Maxbodysize);
		void	setTimeout(int Timeout);
		void	setErrorpages(std::map<int,std::string> Errorpages);
		void	setLocations(std::vector<ServerLocation> Locations);
		void	setServername(std::vector<std::string> Servername);
		void	setIndex(std::vector<std::string> Index);
		void	setRoot(std::string Root);
		void	setPort(int Port);
		void	setHost(in_addr_t Host);

		int	name; // debbug
		// -Methods
		void	addErrorPages(int code, std::string page);
	private:
		int					_maxBodySize;
		int					_timeOut;
		// listen  127.0.0.1:4242
		int					_port;
		in_addr_t			_host;

		// server_name www.localhost localhost;
		std::vector<std::string>	_serverName;

		// TODO
		std::string					_root;

		// index index.html;
		std::vector<std::string>	_index;

		// error_page 404 /404.html;
		// std::pair<int, std::string> errorPage; ??
		std::map<int, std::string>	_errorPages;

		// location / {
		std::vector<ServerLocation>	_locations;



	protected:

};

// -Functions
std::ostream &operator<<(std::ostream &out, Server const &in);

#endif