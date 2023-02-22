#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include <set>
#include <string>
#include <unordered_map>

class Response {
	public:
		// -Constructors
		Response(void);
		Response(Response const &rhs);
		Response(const class Server &server, const class Resquest &request);

		// -Destructor
		~Response(void);

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

		int		_handleRequest(const class Server &server, const class Resquest &request);
		void	_buildResponse();
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