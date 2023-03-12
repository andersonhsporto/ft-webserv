#include "Request.hpp"
#include <sstream>
#include "Utils.hpp"

// -Constructors
Request::Request(void) {
	std::cout << "Request default constructor called\n";
	return ;
}

Request::Request(const std::string &rawRequest) {
	std::cout << "Request default constructor called\n";
	parseRequest(rawRequest);
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
const std::unordered_map<std::string,std::string> &Request::getHeaders(void) const {
	return (this->_headers);
}

const size_t &Request::getBodylength(void) const {
	return (this->_bodyLength);
}

const std::string &Request::getBody(void) const {
	return (this->_body);
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

const std::string &Request::getExtension(void) const {
	return (this->_extension);
}

const std::string &Request::getProtocol(void) const {
	return (this->_protocol);
}

// -Setters
// -Methods
void Request::parseRequest(const std::string &buffer) {
	std::stringstream	ss(buffer);

	int parsed = parseMethod(ss);
	if (parsed == -1) {
		// error handling
	}
	parsed = parseHeader(ss);
	if (parsed == -1) {
		// error handling
	}
	parsed = parsePreBody(ss);
	if (parsed == -1) {
		// error handling
	}
	parsed = parseBody(ss);
	if (parsed == -1) {
		// error handling
	}
}


void Request::execute(const std::string &rawRequest){
	clearVariables();
	parseRequest(rawRequest);
	return ;
}

// -Private Methods
int	Request::parseMethod(std::stringstream &ss) {
	char						newline;
	size_t						pos;
	std::string					method_line;
	std::vector<std::string>	parts;

	std::getline(ss, method_line, '\r');
	parts = utils::splitStringBy(method_line, ' ');
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
	pos = this->_target.rfind('.');
	if (pos == std::string::npos || pos < this->_target.rfind('/')) {
		this->_extension = ".html";
	}
	else {
		this->_extension = this->_target.substr((pos - 1) + 1);
		this->_target.erase(pos);
	}
	ss.get(newline);
	return (0);
}

int	Request::parseHeader(std::stringstream &ss) {
	size_t pos = ss.str().find("\r\n\r\n");
	if (pos == std::string::npos) {
		return (-1);
	}
	std::string headers_str = ss.str().substr(0, pos);
	ss.str().erase(0, pos + 4);
	std::vector<std::string> headers = utils::splitStringBy(headers_str, "\r\n");
	for (const std::string &header : headers) {
		pos = header.find(':');
		if (pos == std::string::npos) {
			return (-1);
		}
		std::string key = header.substr(0, pos);
		std::string value = header.substr(pos + 2);
		utils::trimChar(key, ' ');
		utils::trimChar(value, ' ');
		this->_headers[key] = value;
	}
	return (0);
}

int	Request::parsePreBody(std::stringstream &ss) {
	this->_bodyLength = 0;
	std::unordered_map<std::string, std::string>::iterator it = this->_headers.find("Content-Length");
	if (it != this->_headers.end()) {
		this->_bodyLength = std::stoul(it->second);
	}
	return (0);
}

int	Request::parseBody(std::stringstream &ss) {
	this->_body = ss.str().substr(0, this->_bodyLength);
	ss.str().erase(0, this->_bodyLength);

	return (0);
}

void Request::clearVariables(void){
	this->_bodyLength = 0;
	this->_body = "";
	this->_method = "";
	this->_query = "";
	this->_target = "";
	this->_protocol = "";
	this->_headers.clear();
}

// -Functions
std::ostream &operator<<(std::ostream &out, Request const &in) {
	out << "_method: " << in.getMethod() << "\n"
		<< "_target: " << in.getTarget() << "\n"
		<< "_protocol: " << in.getProtocol() << "\n"
		<< "_body: " << in.getBody() << "\n"
		<< "_header:\n";
	for (std::unordered_map<std::string, std::string>::const_iterator it = in.getHeaders().begin(); it != in.getHeaders().end(); ++it) {
		out << it->first << " " << it->second << "\n";
	}
	return (out);
}
