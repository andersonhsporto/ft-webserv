#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <string>
#include <unordered_map>

class Request {
	public:
		// -Constructors
		Request(void);
		Request(Request const &rhs);
		Request(std::string &rawRequest);

		// -Destructor
		~Request(void);

		// -Operators
		Request &operator=(Request const &rhs);

		// -Getters
		const std::unordered_map<std::string,std::string>	&getHeaders(void) const;
		const size_t		&getBodylength(void) const;
		const std::string	&getBody(void) const;
		const std::string	&getMethod(void) const;
		const std::string	&getQuery(void) const;
		const std::string	&getTarget(void) const;
		const std::string	&getProtocol(void) const;

		// -Setters

		// -Methods
		int	parseRequest(std::string &buffer);
	private:
		size_t								_bodyLength;
		std::string							_body;
		std::string							_method;
		std::string							_query;
		std::string							_target;
		std::string							_protocol;
		std::unordered_map<std::string, std::string>	_headers;

		int	parseMethod(std::stringstream &ss);
		int	parseHeader(std::stringstream &ss);
		int	parsePreBody(std::stringstream &ss);
		int	parseBody(std::stringstream &ss);
};

// -Functions
std::ostream &operator<<(std::ostream &out, Request const &in);

#endif
