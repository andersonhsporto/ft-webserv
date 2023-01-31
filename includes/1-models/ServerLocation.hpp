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
		ServerLocation(int name, const std::string &values);
		ServerLocation(ServerLocation const &rhs);

		// -Destructor
		~ServerLocation(void);

		// -Operators
		ServerLocation &operator=(ServerLocation const &rhs);

		// -Getters
		const bool							&getAutoindex(void) const;
		const std::vector<std::string>		&getRequestshttp(void) const;
		const std::pair<int,std::string>	&getReturnpage(void) const;
		const std::string					&getRoot(void) const;
		const std::string					&getPath(void) const;

		// -Setters
		void	setAutoindex(bool Autoindex);
		void	setRequestshttp(std::vector<std::string> Requestshttp);
		void	setReturnpage(std::pair<int,std::string> Returnpage);
		void	setRoot(std::string Root);
		void	setPath(std::string Path);

		// -Methods
		int	name; // debug
	private:
		std::map<std::string, _parseLocationFunc>	_parseFuncs;
		// /location
		std::string					_path;

		// Return
		std::pair<int, std::string> _returnPage;

		// root /var/www/localhost/htdocs;
		std::string _root;

		// limit_except GET POST;
		std::vector<std::string> _requestsHttp;

		bool	_autoIndex;

		void		_parseValues(const std::string &values);
		static void	_parsePath(const std::string &value, ServerLocation &Location);
		static void	_parseReturn(const std::string &value, ServerLocation &Location);
		static void	_parseRoot(const std::string &value, ServerLocation &Location);
		static void	_parseRequest(const std::string &value, ServerLocation &Location);
		static void	_parseAutoIndex(const std::string &value, ServerLocation &Location);


	protected:

};

// -Functions
std::ostream &operator<<(std::ostream &out, ServerLocation const &in);

#endif