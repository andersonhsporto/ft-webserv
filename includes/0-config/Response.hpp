#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include <string>
#include <unordered_map>

class Response {
	public:
		// -Constructors
		Response(void);
		Response(Response const &rhs);
		Response(const std::string &rawResponse);

		// -Destructor
		~Response(void);

		// -Operators
		Response &operator=(Response const &rhs);

		// -Getters
		const std::unordered_map<std::string,std::string>	&getHeaders(void) const;
		const int											&getStatuscode(void) const;
		const std::string									&getVersion(void) const;
		const std::string									&getStatusmessage(void) const;
		const std::string									&getBody(void) const;

		// -Setters

		// -Methods

	private:
		int												_statusCode;
		std::string										_version;
		std::string										_statusMessage;
		std::string										_body;
		std::unordered_map<std::string, std::string>	_headers;
	
		void	parseRawResponse(const std::string &rawResponse);
};

// -Functions
std::ostream &operator<<(std::ostream &out, Response const &in);

#endif