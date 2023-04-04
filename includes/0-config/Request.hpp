#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <string>
#include <map>

class Request {
	public:
		// -Constructors
		Request(void);
		Request(Request const &rhs);
		Request(const std::string &rawRequest);

		// -Destructor
		~Request(void);

		// -Operators
		Request &operator=(Request const &rhs);

		// -Getters
		const std::map<std::string,std::string>	&getHeaders(void) const;
		const size_t		&getBodylength(void) const;
		const std::string	&getBody(void) const;
		const std::string	&getMethod(void) const;
		const std::string	&getQuery(void) const;
		const std::string	&getTarget(void) const;
		const std::string	&getExtension(void) const;
		const std::string	&getProtocol(void) const;

		// -Setters

		// -Methods
		void parseRequest(const std::string &buffer);

	private:
		size_t											_bodyLength;
		std::string										_body;
		std::string										_method;
		std::string										_query;
		std::string										_target;
		std::string										_extension;
		std::string										_protocol;
		std::map<std::string, std::string>	_headers;

		void	parseMethod(std::stringstream &ss);
		void	parseHeader(std::stringstream &ss);
		void	parsePreBody(void);
		void	parseBody(std::stringstream &ss);
};

// -Functions
std::ostream &operator<<(std::ostream &out, Request const &in);

#endif
