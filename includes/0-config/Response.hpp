#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <set>
#include <string>
#include <iostream>
#include <unordered_map>
#include "Autoindex.hpp"

class Response {
	public:
		// -Constructors
		Response(void);
		Response(Response const &rhs);
		Response(const class Server &server, const class Request &request);

		// -Destructor
		~Response();

		// -Operators
		Response &operator=(Response const &rhs);

		// -Getters
		const std::string	&getRawresponse(void) const;

		// -Setters

		// -Methods

	private:
		size_t											_bodyLength;
		std::string										_body;
		std::string										_rawResponse;
		std::pair<std::string, std::string>				_status;
		std::unordered_map<std::string, std::string>	_headers;
		std::set<std::string>							_allowedMethods;
		Autoindex										_autoindex;

		int			_handleRequest(const class Server &server, const class Request &request);
		void		_buildResponse();
		void		_setResponseVariables(const class Server &server, const class Request &request);
		void		_setStatus(const std::string& code);
		void		_setErrorResponse(const class Server &server);
		std::string	_getContentTypeHeader(const std::string& filePath);
		int			_getMethodHTTP(const Request &request, const Server &server, std::string &root, const bool &isRootLocation);
		int			_postMethodHTTP(const Request &request, const Server &server, std::string &root, const bool &isRootLocation);
		int			_deleteMethodHTTP(const Request &request, const Server &server, std::string &root, const bool &isRootLocation);
		int			_applyMethodHTTP(const Request &request, const Server &server, std::string &root, const bool &isRootLocation);
		std::string _getPageFile(const Request &request, const Server &server, std::string path, const bool &isRootLocation);
		std::string _getPageAutoindex(const std::string &path, const Server &server);
		int 		_calculateBodySize(const std::string &body) const;
};

// -Functions
std::ostream &operator<<(std::ostream &out, Response const &in);

#endif