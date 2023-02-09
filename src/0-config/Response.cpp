#include "Response.hpp"

// -Constructors
Response::Response(void) {
	std::cout << "Response default constructor called\n";
	return ;
}

Response::Response(Response const &rhs) {
	std::cout << "Response copy constructor called\n";
	*this = rhs;
	return ;
}

Response::Response(const std::string &rawResponse) {
	std::cout << "Response String constructor called\n";
	parseRawResponse(rawResponse);
}

// -Destructor
Response::~Response(void) {
	std::cout << "Response default destructor called\n";
	return ;
}

// -Operators
Response &Response::operator=(Response const &rhs) {
	if (this != &rhs) {
		std::cout << "Response copy assignment operator called\n";
	}
	return (*this);
}

// -Getters
// -Setters
// -Methods
// -Private Methods
void parseRawResponse(const std::string &rawResponse) {
	std::stringstream ss(rawResponse);
	std::string line;

	// Parse the first line to get status code, version and status message
	if (std::getline(ss, line)) {
		std::stringstream firstLine(line);
		firstLine >> this->_version >> this->_statusCode >> this->_statusMessage;
	}

	// Parse headers
	while (std::getline(ss, line) && !line.empty()) {
		std::string key, value;
		auto separatorPos = line.find(':');
		key = line.substr(0, separatorPos);
		value = line.substr(separatorPos + 2);
		this->_headers[key] = value;
	}

	// Get the body
	this->_body = ss.str();
	this->_body = this->_body.substr(line.length() + 1);
}

// -Functions
std::ostream &operator<<(std::ostream &out, Response const &in) {
	(void)in;
	return (out);
}
