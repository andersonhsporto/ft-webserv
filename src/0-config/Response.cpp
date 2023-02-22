#include "Response.hpp"
#include "Request.hpp"
#include "Server.hpp"

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

Response::Response(const Server &server, const Resquest &request) : _allowedMethods({"GET", "POST", "DELETE"}) {
	std::cout << "Response class request constructor called\n";
	if (_handleRequest(server, request) != -1)
		_buildResponse();
	/*
		else error handling
	*/
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
	/*
		Pegar Status Code
		Pegar Status Message
	*/
	response_stream << "HTTP/1.1" << " " \
	<< this->_status.first << " " \
	<< this->_status.second << "\r\n";

	/*
		Gerar Headers
	*/
	// Add headers
	for (auto& header : this->_headers) {
		response_stream << header.first << ": " << header.second << "\r\n";
	}

	/*
		Gerar body
	*/
	// Add content length if body is not empty
	if (!this->_body.empty()) {
		response_stream << "Content-Length: " << this->_bodyLength << "\r\n";
	}

	// Add empty line to separate headers from body
	response_stream << "\r\n";

	// Add body
	response_stream << this->_body;
	this->_rawResponse = response_stream.str();
}

int Response::_handleRequest(const Server &server, const Resquest &request) {
	/*

		ERROR CHECKING WIP

	// - Check Protocol
	if (request.getProtocol()!= "HTTP/1.1") {
		// Error Protocol not allowed
		return;
	}
	// - Check Method
	if (_allowedMethods.count(request.getMethod()) == 0) {
		// Error method not allowed
		return;
	}
	// - Check body Lenght
	if (request.getBodyLength() > server.getMaxBodySize()) {
		// Error size too large
		return;
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
		// Error Not Found
		return;
	}
	// Execute any relevant CGI scripts ????????
	if (location->getAutoIndex()) {
		std::string	indexFile;
		bool		found = false;
		for (std::vector<std::string>::const_iterator it = server.getIndex().begin(); it != server.getIndex().end(); ++it) {
			std::string path = server.getRoot() + request.getTarget() + "/" + *it;
			std::ifstream file(path);
			if (file.good()) {
				outFile = path;
				found = true;
			}
		}
		if (found) {
			response.setStatusCode(301, "Moved Permanently");
			response.addHeader("Location", indexFile);
			return;
		}
	}
	std::string indexFile;
	if (findIndexFile(request, serverConfig.getRoot(), location->getIndex(), indexFile)) {
		response.setStatusCode(301, "Moved Permanently");
		response.addHeader("Location", indexFile);
		return;
	}
	// Try to serve the requested file
	std::string filePath = serverConfig.getRoot() + request.getTarget();
	if (serveFile(filePath, response)) {
		response.setStatusCode(200, "OK");
	} else {
		response.setStatusCode(404, "Not Found");
	}
	*/
	/*
		Making Get no errors work
		Request
		size_t											_bodyLength;
		std::string										_body;
		std::string										_method;
		std::string										_query;
		std::string										_target;
		std::string										_protocol;

		Server
		int									_maxBodySize;
		int									_timeOut;
		int									_port;
		in_addr_t							_host;
		std::string							_root;
		std::vector<std::string>			_index;
		std::vector<std::string>			_serverName;
		std::vector<ServerLocation*>		_locations;
		std::map<int, std::string>			_errorPages;
		// std::map<std::string, std::string>	_cgi;

		Contruir Body
		HTTP/1.1 -> Protocol
		200 -> Code
		OK -> Code Message

		Body e o Content-Type (header) vem do arquivo.

	*/
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