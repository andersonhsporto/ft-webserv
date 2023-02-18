#ifndef SERVERLOCATION_HPP
#define SERVERLOCATION_HPP

#include <iostream>
#include <vector>
#include <map>
#include <sstream>

class ServerLocation {
	private:
		// -Typedefs
		typedef void (*_parseLocationFunc)(const std::string &values, ServerLocation &Location);

	public:
		// -Constructors
		ServerLocation(void);
		ServerLocation(const std::string &values);
		ServerLocation(ServerLocation const &rhs);

		// -Destructor
		~ServerLocation(void);

		// -Operators
		ServerLocation &operator=(ServerLocation const &rhs);

		// -Getters
		const std::string					&getRoot(void) const;
		const std::string					&getPath(void) const;
		const bool							&getAutoindex(void) const;
		const std::pair<int,std::string>	&getReturnpage(void) const;
		const std::vector<std::string>		&getRequestshttp(void) const;

		// -Setters
		void	setRoot(std::string Root);
		void	setPath(std::string Path);
		void	setAutoindex(bool Autoindex);
		void	setRequestshttp(std::vector<std::string> Requestshttp);
		void	setReturnpage(std::pair<int,std::string> Returnpage);

		// -Methods
	private:
		std::string									_path;
		std::string									_root;
		bool										_autoIndex;
		std::vector<std::string>					_requestsHttp;
		std::pair<int, std::string>					_returnPage;
		std::map<std::string, _parseLocationFunc>	_parseFuncs;

		void		_parseValues(const std::string &values);
		static void	_parsePath(const std::string &value, ServerLocation &Location);
		static void	_parseReturn(const std::string &value, ServerLocation &Location);
		static void	_parseRoot(const std::string &value, ServerLocation &Location);
		static void	_parseRequest(const std::string &value, ServerLocation &Location);
		static void	_parseAutoIndex(const std::string &value, ServerLocation &Location);
};

// -Functions
std::ostream &operator<<(std::ostream &out, ServerLocation const &in);

#endif