#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <set>
#include <string>
#include <iostream>
#include <unordered_map>

class Response {
	public:
		// -Constructors
		Response(void);
		Response(Response const &rhs);
		Response(const class Server &server, const class Request &request);

		// -Destructor
		~Response(void);

		// -Operators
		Response &operator=(Response const &rhs);

		// -Getters
		const std::string	&getRawresponse(void) const;

		// -Setters

		// -Methods
		void execute (const class Server &server, const class Request &request);

	private:
		size_t											_bodyLength;
		std::string										_body;
		std::string										_rawResponse;
		std::pair<std::string, std::string>				_status;
		std::unordered_map<std::string, std::string>	_headers;
		std::set<std::string>							_allowedMethods;

		int			_handleRequest(const class Server &server, const class Request &request);
		void		_buildResponse();
		void		_setResponseVariables(const class Server &server, const class Request &request);
		void		_setStatus(const std::string& code);
		void		_setErrorResponse(const class Server &server);
		std::string	_getContentTypeHeader(const std::string& filePath);
		void		_clearVariables(void);
};

// -Functions
std::ostream &operator<<(std::ostream &out, Response const &in);

#endif

/*
	FOR CLIENT
public:
	const std::unordered_map<std::string,std::string>	&getHeaders(void) const;
	const int											&getStatuscode(void) const;
	const std::string									&getVersion(void) const;
	const std::string									&getStatusmessage(void) const;
	const std::string									&getBody(void) const;

private:

	int												_statusCode;
	std::string										_version;
	std::string										_statusMessage;
	std::string										_body;
	std::unordered_map<std::string, std::string>	_headers;

	void	parseRawResponse(const std::string &rawResponse);


*/