#include "Request.hpp"
#include "ParserUtils.hpp"

// -Constructors
Request::Request(void) {
	std::cout << "Request default constructor called\n";
	return ;
}

Request::Request(Request const &rhs) {
	std::cout << "Request copy constructor called\n";
	*this = rhs;
	return ;
}

// -Destructor
Request::~Request(void) {
	std::cout << "Request default destructor called\n";
	return ;
}

// -Operators
Request &Request::operator=(Request const &rhs) {
	if (this != &rhs) {
		std::cout << "Request copy assignment operator called\n";
	}
	return (*this);
}

// -Getters
const std::map<std::string,std::string> &Request::getHeaders(void) const {
	return (this->_headers);
}

const size_t &Request::getBodylength(void) const {
	return (this->_bodyLength);
}

const std::string &Request::getBody(void) const {
	return (this->_body);
}

const std::string &Request::getBuffer(void) const {
	return (this->_buffer);
}

const std::string &Request::getMethod(void) const {
	return (this->_method);
}

const std::string &Request::getQuery(void) const {
	return (this->_query);
}

const std::string &Request::getTarget(void) const {
	return (this->_target);
}

const std::string &Request::getProtocol(void) const {
	return (this->_protocol);
}

// -Setters
// -Methods
int	Request::parseRequest(std::string &buffer) {
	this->_buffer = buffer;
	int parsed = parseMethod();
	if (parsed == -1) {
		return (parsed);
	}
	parsed = parseHeader();
	if (parsed == -1) {
		return (parsed);
	}
	parsed = parsePreBody();
	if (parsed == -1) {
		return (parsed);
	}
	parsed = parseBody();
	return (parsed);
}

// -Private Methods
int	Request::parseMethod(void) {
	size_t pos = this->_buffer.find("\r\n");
	if (pos == std::string::npos) {
		return (-1);
	}
	std::string method_line = this->_buffer.substr(0, pos);
	this->_buffer.erase(0, pos + 2);
	std::vector<std::string> parts = parser::splitStringBy(method_line, ' ');
	if (parts.size() != 3) {
		return (-1);
	}
	this->_method = parts[0];
	this->_target = parts[1];
	this->_protocol = parts[2];
	pos = this->_target.find('?');
	if (pos != std::string::npos) {
		this->_query = this->_target.substr(pos + 1);
		this->_target.erase(pos);
	}
	return ((int)pos + 2); // Return how many char were parsed
}

int	Request::parseHeader(void) {
	size_t pos = this->_buffer.find("\r\n\r\n");
	if (pos == std::string::npos) {
		return (-1);
	}
	std::string headers_str = this->_buffer.substr(0, pos);
	this->_buffer.erase(0, pos + 4);
	std::vector<std::string> headers = parser::splitStringBy(headers_str, "\r\n");
	for (const std::string &header : headers) {
		pos = header.find(':');
		if (pos == std::string::npos) {
			return (-1);
		}
		std::string key = header.substr(0, pos);
		std::string value = header.substr(pos + 2);
		parser::trimChar(key, ' ');
		parser::trimChar(value, ' ');
		this->_headers[key] = value;
	}
	return ((int)pos + 4);  // Return how many char were parsed
}

int	Request::parsePreBody(void) {
	this->_bodyLength = 0;
	std::map<std::string, std::string>::iterator it = this->_headers.find("Content-Length");
	if (it != this->_headers.end()) {
		this->_bodyLength = std::stoul(it->second);
	}
	return (0);
}

int	Request::parseBody(void) {
	this->_body = this->_buffer.substr(0, this->_bodyLength);
	this->_buffer.erase(0, this->_bodyLength);

	return ((int)this->_bodyLength);  // Return how many char were parsed
}

// -Functions
std::ostream &operator<<(std::ostream &out, Request const &in) {
	out << "_method: " << in.getMethod() << "\n"
		<< "_target: " << in.getTarget() << "\n"
		<< "_protocol: " << in.getProtocol() << "\n"
		<< "_body: " << in.getBody() << "\n"
		<< "_header:\n";
	for (std::map<std::string, std::string>::const_iterator it = in.getHeaders().begin(); it != in.getHeaders().end(); ++it) {
		out << it->first << " " << it->second << "\n";
	}
	return (out);
}
