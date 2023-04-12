#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <vector>
#include <map>
#include <arpa/inet.h>
#include "Socket.hpp"
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
		const bool							&getAutoindex(void) const;
		const std::map<std::string,std::string>	&getCgi(void) const;
		const int							&getMaxbodysize(void) const;
		const int							&getTimeout(void) const;
		const std::map<int,std::string>		&getErrorpages(void) const;
		const std::vector<ServerLocation*>	&getLocations(void) const;
		const std::vector<std::string>		&getServername(void) const;
		const std::vector<std::string>		&getIndex(void) const;
		const std::string					&getRoot(void) const;
        const std::string                   &getCGIFolder(void) const;
		const int							&getPort(void) const;
		const std::string					&getHost(void) const;
		Socket								getListener(void) const;

		// -Setters
		void	setAutoindex(bool autoindex);
		void	setCgi(std::map<std::string,std::string> Cgi);
		void	setMaxbodysize(int Maxbodysize);
		void	setTimeout(int Timeout);
		void	setErrorpages(std::map<int,std::string> Errorpages);
		void	setLocations(std::vector<ServerLocation*> Locations);
		void	setServername(std::vector<std::string> Servername);
		void	setIndex(std::vector<std::string> Index);
		void	setRoot(std::string Root);
        void    setCGIFolder(std::string CGIFolder);
		void	setPort(int Port);
		void	setHost(std::string Host);

		// -Methods
		void	addErrorPages(const int &code, const std::string &page);
		void	addLocations(const std::string &values);
		void	addCgi(const std::string &extension, const std::string &path);

	private:
		int									_maxBodySize;
		int									_timeOut;
		int									_port;
		bool								_autoindex;
		Socket								_listener;
		std::string							_host;
		std::string							_root;
        std::string                         _CGIFolder;
		std::vector<std::string>			_index;
		std::vector<std::string>			_serverName;
		std::vector<ServerLocation*>		_locations;
		std::map<int, std::string>			_errorPages;
		std::map<std::string, std::string>	_cgi;
};

// -Functions
std::ostream &operator<<(std::ostream &out, Server const &in);

#endif
