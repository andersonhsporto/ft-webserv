#include "Response.hpp"
#include <fstream>
#include "ServerLocation.hpp"
#include "Request.hpp"
#include "Server.hpp"
#include "Utils.hpp"

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

Response::Response(const Server &server, const Request &request) : _allowedMethods({"GET", "POST", "DELETE"}) {
	std::cout << "Response class request constructor called\n";
	if (_handleRequest(server, request) != -1) {
		_setResponseVariables(server, request);
	}
	else {
		_setErrorResponse(server);
	}
	_buildResponse();
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
const std::string	&Response::getRawresponse(void) const {
	return (this->_rawResponse);
}

// -Setters
// -Methods
// -Private Methods
void Response::_buildResponse() {
	std::stringstream response_stream;

	response_stream << "HTTP/1.1" << " " \
	<< this->_status.first << " " \
	<< this->_status.second << "\r\n";
	for (std::unordered_map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); ++it) {
		response_stream << it->first << ": " << it->second << "\r\n";
	}
	response_stream << "\r\n";
	response_stream << this->_body;
	this->_rawResponse = response_stream.str();
}

void Response::_setResponseVariables(const Server &server, const Request &request) {
	this->_headers["Date"] = utils::getCurrentTime();
	if (!request.getTarget().empty()) {
		std::string root;
		const std::vector<ServerLocation*> &locations = server.getLocations();
		for (std::vector<ServerLocation*>::const_iterator it = locations.begin(); it != locations.end(); ++it) {
			if ((*it)->getPath() == request.getTarget()) {
				root = (*it)->getRoot();
				if ((*it)->getAutoindex()) {
					std::string	indexFile;
					bool		found = false;
					for (std::vector<std::string>::const_iterator it = server.getIndex().begin(); it != server.getIndex().end(); ++it) {
						std::string path = server.getRoot() + request.getTarget() + "/" + *it;
						std::ifstream file(path);
						if (file.good())
							found = true;
					}
					if (found) {
						_setStatus("301");
						this->_headers["Location"] = indexFile;
					}
				}
				if ((*it)->getReturnpage().first != 0 || !(*it)->getReturnpage().second.empty()) {
					_setStatus("302");
					this->_headers["Location"] = (*it)->getReturnpage().second;
				}
				break;
			}
		}
		utils::fileToString(root + request.getTarget(), this->_body);
		if (!this->_body.empty()) {
			this->_headers["Content-Length"] = utils::intToString(this->_body.size());
			this->_headers["Content-Type"] = this->_getContentTypeHeader(root + request.getTarget());
		}
	}
	if (!server.getServername().empty()) {
		for (std::vector<std::string>::const_iterator it = server.getServername().begin(); it != server.getServername().end(); ++it) {
			this->_headers["Server"].append(*it);
			if (it != server.getServername().end() - 1) {
				this->_headers["Server"].append(", ");
			}
		}
	}
}

void Response::_setErrorResponse(const Server &server) {
	std::string errorPage;

	if (server.getErrorpages().count(std::stoi(this->_status.first)) > 0) {
		errorPage = server.getErrorpages().at(std::stoi(this->_status.first));
	} else {
		errorPage = "<html><head><title>" + this->_status.first + \
		" " + this->_status.second + "</title></head><body><h1>" + \
		this->_status.first + " " + this->_status.second + \
		"</h1></body></html>";
	}
	this->_headers["Content-Type"] = "text/html";
	this->_headers["Content-Length"] = std::to_string(errorPage.length());
	this->_body = errorPage;
}

std::string Response::_getContentTypeHeader(const std::string& filePath) {
	std::unordered_map<std::string, std::string> extensionMap {
		{ "html", "text/html" },
		{ "htm", "text/html" },
		{ "css", "text/css" },
		{ "js", "application/javascript" },
		{ "json", "application/json" },
		{ "jpg", "image/jpeg" },
		{ "jpeg", "image/jpeg" },
		{ "png", "image/png" },
		{ "gif", "image/gif" }
	};
	std::string extension = filePath.substr(filePath.find_last_of(".") + 1);
	std::string contentType = extensionMap[extension];

	if (contentType.empty()) {
		contentType = "application/octet-stream";
	}
	return (contentType);
}

void Response::_setStatus(const std::string& code) {
	std::unordered_map<std::string, std::string> messages {
		{ "200", "OK" },
		{ "201", "Created" },
		{ "203", "No Content" },
		{ "301", "Moved Permanently" },
		{ "302", "Found" },
		{ "400", "Bad Request" },
		{ "403", "Forbidden" },
		{ "404", "Not Found" },
		{ "405", "Method Not Allowed" },
		{ "413", "Entity Too Large" },
		{ "500", "Internal Server Error" },
		{ "505", "Version Not Supported" }
	};
	this->_status = std::make_pair(code, messages[code]);
}

int Response::_handleRequest(const Server &server, const Request &request) {
	// - Check Protocol
	if (request.getProtocol()!= "HTTP/1.1") {
		_setStatus("505");
		return (-1);
	}
	// - Check Method
	if (_allowedMethods.count(request.getMethod()) == 0) {
		_setStatus("405");
		return (-1);
	}
	// - Check body Lenght
	if (request.getBodylength() > server.getMaxbodysize()) {
		_setStatus("413");
		return (-1);
	}
	// - Check Location
	const ServerLocation* location = nullptr;
	for (std::vector<ServerLocation*>::const_iterator it = server.getLocations().begin(); it != server.getLocations().end(); ++it) {
		if ((*it)->getPath() == request.getTarget()) {
			location = *it;
			break;
		}
	}
	if (!location) {
		_setStatus("404");
		return (-1);
	}
	// Execute any relevant CGI scripts
	std::string filePath = server.getRoot() + request.getTarget();
	std::ifstream input(filePath.c_str());
	if (!input) {
		_setStatus("404");
		return (-1);
	}
	else {
		_setStatus("200");
	}
	return (0);
}

// -Functions
std::ostream &operator<<(std::ostream &out, Response const &in) {
	(void)in;
	return (out);
}

/*
	FOR CLIENT
#include "ParserUtils.hpp"

Response::Response(const std::string &rawResponse) {
	std::cout << "Response String constructor called\n";
	parseRawResponse(rawResponse);
}

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
*/