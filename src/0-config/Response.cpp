#include "Response.hpp"
#include "ParserUtils.hpp"

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
const std::unordered_map<std::string,std::string> &Response::getHeaders(void) const {
	return (this->_headers);
}

const int &Response::getStatuscode(void) const {
	return (this->_statusCode);
}

const std::string &Response::getVersion(void) const {
	return (this->_version);
}

const std::string &Response::getStatusmessage(void) const {
	return (this->_statusMessage);
}

const std::string &Response::getBody(void) const {
	return (this->_body);
}

// -Setters
// -Methods
// -Private Methods
void Response::parseRawResponse(const std::string &rawResponse) {
	std::stringstream	ss(rawResponse);
	std::string			line;
	std::string			key;
	std::string			value;

	// Parse the first line to get status code, version and status message
	if (std::getline(ss, line)) {
		std::stringstream firstLine(line);
		firstLine >> this->_version >> this->_statusCode >> this->_statusMessage;
	}

    // Parse headers
    while (std::getline(ss, line) && line != "\r") {
        auto separatorPos = line.find(':');
        key = line.substr(0, separatorPos);
        value = line.substr(separatorPos + 2, line.size() - separatorPos - 3);
        parser::trimChar(key, ' ');
        parser::trimChar(value, ' ');
        this->_headers[key] = value;
    }

    // Get the body
    while (std::getline(ss, line)) {
        this->_body += line + "\n";
    }
}


// -Functions
std::ostream &operator<<(std::ostream &out, Response const &in) {
		out << "_statusCode: " << in.getStatuscode() << "\n"
		<< "_version: " << in.getVersion() << "\n"
		<< "_statusMessage: " << in.getStatusmessage() << "\n"
		<< "_body:\n" << in.getBody() << "\n"
		<< "_header:\n";
	for (std::unordered_map<std::string, std::string>::const_iterator it = in.getHeaders().begin(); it != in.getHeaders().end(); ++it) {
		out << it->first << " " << it->second << "\n";
	}
	return (out);
}
