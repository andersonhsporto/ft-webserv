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
Response::~Response() {
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
void Response::execute(const class Server &server, const class Request &request){
	_clearVariables();
	_allowedMethods = {"GET", "POST", "DELETE"};
	if (_handleRequest(server, request) != -1) {
		_setResponseVariables(server, request);
	}
	else {
		_setErrorResponse(server);
	}
	_buildResponse();
}

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
		const std::vector<ServerLocation*> &locations = server.getLocations();
		for (std::vector<ServerLocation*>::const_iterator it = locations.begin(); it != locations.end(); ++it) {
			if ((*it)->getPath() == request.getTarget()) {
				if ((*it)->getAutoindex()) {
					std::cout << "OLAAAAAAAAAA\n";
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
		this->_headers["Content-Length"] = utils::intToString(this->_body.size());
		this->_headers["Content-Type"] = this->_getContentTypeHeader(request.getExtension());
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

std::string _buildErrorPage(const std::string codeError, const std::string textError){
	std::stringstream pageError;
	pageError << "<!DOCTYPE html><html><head><meta content=\"text/html;charset=utf-8\" http-equiv=\"Content-Type\"><meta content=\"utf-8\" http-equiv=\"encoding\"><style>@import url('https://fonts.googleapis.com/css2?family=Press+Start+2P&display=swap');"\
	<< "* { padding: 0; margin: 0; box-sizing: border-box; font-family: 'Press Start 2P'; color: #FFFFFF; text-align: center; }"\
	<< "body {background-color: #000000;background-image: url(\"data:image/svg+xml,%3Csvg xmlns='http://www.w3.org/2000/svg' width='42' height='58' viewBox='0 0 42 58'%3E%3Cg fill='%23dddcdd' fill-opacity='0.23'%3E%3Cpath fill-rule='evenodd' d='M12 18h12v18h6v4H18V22h-6v-4zm-6-2v-4H0V0h36v6h6v36h-6v4h6v12H6v-6H0V16h6zM34 2H2v8h24v24h8V2zM6 8a2 2 0 1 0 0-4 2 2 0 0 0 0 4zm8 0a2 2 0 1 0 0-4 2 2 0 0 0 0 4zm8 0a2 2 0 1 0 0-4 2 2 0 0 0 0 4zm8 0a2 2 0 1 0 0-4 2 2 0 0 0 0 4zm0 8a2 2 0 1 0 0-4 2 2 0 0 0 0 4zm0 8a2 2 0 1 0 0-4 2 2 0 0 0 0 4zm0 8a2 2 0 1 0 0-4 2 2 0 0 0 0 4zM2 50h32v-8H10V18H2v32zm28-6a2 2 0 1 0 0 4 2 2 0 0 0 0-4zm-8 0a2 2 0 1 0 0 4 2 2 0 0 0 0-4zm-8 0a2 2 0 1 0 0 4 2 2 0 0 0 0-4zm-8 0a2 2 0 1 0 0 4 2 2 0 0 0 0-4zm0-8a2 2 0 1 0 0 4 2 2 0 0 0 0-4zm0-8a2 2 0 1 0 0 4 2 2 0 0 0 0-4zm0-8a2 2 0 1 0 0 4 2 2 0 0 0 0-4z'/%3E%3C/g%3E%3C/svg%3E\");}"\
	<< "section.notFound {display: flex; justify-content: center; align-items: center; margin: 0 5%; height: 100vh;}"
	<< "section.notFound h1 { color: red; font-size: 100px;} section.notFound h2 {font-size: 50px;} section.notFound h1, h2, h3 {margin-bottom: 40px;} div.text {height: 50vh;}"\
	<<  "div.text a {text-decoration: none;margin-right: 20px;}div.text a:hover {color: red;text-decoration: underline;}"
	<< "@media only screen and (max-width: 768px) {section.notFound {flex-direction: column;justify-content: space-around;}section.notFound div.img img {width: 70vw;height: auto;}"\
    << "section.notFound h1 {font-size: 50px;}section.notFound h2 {font-size: 25px;}div.text a:active {color: red;text-decoration: underline; }}"
	<< "</style></head><body><section class=\"notFound\"><div class=\"img\">"
    << "<img src=\"https://assets.codepen.io/5647096/backToTheHomepage.png\" alt=\"Back to the Homepage\"/>"\
    << "<img src=\"https://assets.codepen.io/5647096/Delorean.png\" alt=\"El Delorean, El Doc y Marti McFly\"/></div><div class=\"text\">"\
	<< "<h1>" << codeError << "</h1>" << "<h2>" << textError << "</h2>" << "</div> </section> </body> </html>";
	return pageError.str();
}

void Response::_setErrorResponse(const Server &server) {
	std::string errorPage;

	this->_headers["Date"] = utils::getCurrentTime();
	if (server.getErrorpages().count(std::stoi(this->_status.first)) > 0) {
		if(utils::fileToString(server.getErrorpages().at(std::stoi(this->_status.first)), errorPage) == -1)
			errorPage = server.getErrorpages().at(std::stoi(this->_status.first));
	} else {
		errorPage = _buildErrorPage(this->_status.first, this->_status.second);
	}
	this->_headers["Content-Type"] = "text/html";
	this->_headers["Content-Length"] = std::to_string(errorPage.length());
	this->_body = errorPage;
	if (!server.getServername().empty()) {
		for (std::vector<std::string>::const_iterator it = server.getServername().begin(); it != server.getServername().end(); ++it) {
			this->_headers["Server"].append(*it);
			if (it != server.getServername().end() - 1) {
				this->_headers["Server"].append(", ");
			}
		}
	}
}

std::string Response::_getContentTypeHeader(const std::string& extension) {
	std::unordered_map<std::string, std::string> extensionMap {
		{ ".html", "text/html" },
		{ ".htm", "text/html" },
		{ ".css", "text/css" },
		{ ".js", "application/javascript" },
		{ ".json", "application/json" },
		{ ".jpg", "image/jpeg" },
		{ ".jpeg", "image/jpeg" },
		{ ".png", "image/png" },
		{ ".gif", "image/gif" }
	};
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
		{ "204", "No Content" },
		{ "301", "Moved Permanently" },
		{ "302", "Found" },
		{ "400", "Bad Request" },
		{ "403", "Forbidden" },
		{ "404", "Not Found" },
		{ "405", "Method Not Allowed" },
		{ "413", "Entity Too Large" },
		{ "422", "Unprocessable Content" },
		{ "500", "Internal Server Error" },
		{ "505", "Version Not Supported" }
	};
	this->_status = std::make_pair(code, messages[code]);
}

std::string Response::_getPageFile(const Request &request, const Server &server, std::string path, const bool &isRootLocation){
	if(utils::pathIs(path) == "dir" || (request.getTarget() != "/" && !server.getAutoindex() && !isRootLocation))
		return "403";
	if (utils::fileToString(path, this->_body) == -1) 
		return "404";
	else if (this->_body.empty())
		return "204";
	return "200";
}

std::string Response::_getPageAutoindex(const std::string &path, const Server &server){
	return _autoindex.autoindexPageGenerator(path, this->_body, server);
}

int	Response::_getMethodHTTP(const Request &request, const Server &server, std::string &root, const bool &isRootLocation){
	std::string path = "";

	if (request.getTarget() == "/") {
		for (std::vector<std::string>::const_iterator it = server.getIndex().begin(); it != server.getIndex().end(); it++) {
			if(utils::fileExist(root + "/" + *it)){
				std::cout << "file /:" << root + "/" + *it << "\n";
				path = root + "/" + *it;
				break;
			}
		}
	}
	else {
		if(utils::fileExist(root + request.getTarget() + request.getExtension())){
			path  = root + request.getTarget() + request.getExtension();
			std::cout << "file:" << root + request.getTarget() + request.getExtension() << "\n";
		}
	}
	if (path.empty())
		path = root + request.getTarget();
	if (utils::pathIs(path) == "dir" && server.getAutoindex())
		_setStatus(_getPageAutoindex(request.getTarget(), server));
	else
		_setStatus(_getPageFile(request, server, path, isRootLocation));
	return (this->_status.first == "200" ? 0 : -1);
}

int	Response::_postMethodHTTP(const Request &request, const Server &server, std::string &root, const bool &isRootLocation){
	std::string path;
	if(request.getTarget() == "/"){
		for (std::vector<std::string>::const_iterator it = server.getIndex().begin(); it != server.getIndex().end(); it++) {
			if(!(utils::fileExist(root + "/" + *it)))
				continue;
			path = root + "/" + *it;
			break;
		};
		if(path.empty())
			_setStatus("404");
		else if(utils::insertStringIntoFile(path, request.getBody()))
			_setStatus("200");
		else
			_setStatus("403");
	} else {
		if(isRootLocation)
			path = root + request.getTarget() + request.getExtension();
		else if(server.getAutoindex())
			path = root + request.getTarget();

		if(path.empty() && utils::fileExist(root + request.getTarget()))
			_setStatus("403");
		else if (!(utils::fileExist(path)))
			_setStatus("404");
		else if(utils::insertStringIntoFile(path, request.getBody()))
			_setStatus("200");
		else
			_setStatus("422");
	}
	return (this->_status.first == "200" ? 0 : -1);
}

int	Response::_deleteMethodHTTP(const Request &request, const Server &server, std::string &root, const bool &isRootLocation){
	std::string path;
	if(request.getTarget() == "/"){
		for (std::vector<std::string>::const_iterator it = server.getIndex().begin(); it != server.getIndex().end(); it++) {
			if(!(utils::fileExist(root + "/" + *it)))
				continue;
			path = root + "/" + *it;
			break;
		}
		if(path.empty())
			_setStatus("404");
		else if(std::remove(path.c_str()) == 0)
			_setStatus("200");
		else
			_setStatus("403");
	}
	else {
		if(isRootLocation)
			path = root + request.getTarget() + request.getExtension();
		else if(server.getAutoindex())
			path = root + request.getTarget();
		
		if (path.empty() && utils::fileExist(root + request.getTarget()))
			_setStatus("403");
		else if (!(utils::fileExist(path)))
			_setStatus("404");
		else if(std::remove(path.c_str()) == 0)
			_setStatus("200");
		else
			_setStatus("422");
	}
	return (this->_status.first == "200" ? 0 : -1);
}

int Response::_applyMethodHTTP(const Request &request, const Server &server, std::string &root, const bool &isRootLocation){
	if(request.getMethod() == "GET")
		return _getMethodHTTP(request, server, root, isRootLocation);
	else if (request.getMethod() == "POST")
		return _postMethodHTTP(request, server, root, isRootLocation);
	else if(request.getMethod() == "DELETE")
		return _deleteMethodHTTP(request, server, root, isRootLocation);
	return (0);
}

int Response::_calculateBodySize(const std::string &body) const {
	return body.size();
}

int Response::_handleRequest(const Server &server, const Request &request) {
	int			outRead = -1;
	bool		isRootLocation;
	std::string	root;

	root = server.getRoot();
	isRootLocation = false;
	// - Check Protocol
	if (request.getProtocol()!= "HTTP/1.1") {
		_setStatus("505");
		return (-1);
	}
	if (_calculateBodySize(request.getBody()) > (server.getMaxbodysize() * 1024)){
		_setStatus("413");
		return (-1);
	}
	// - Check Location
	const ServerLocation* location = nullptr;
	for (std::vector<ServerLocation*>::const_iterator it = server.getLocations().begin(); it != server.getLocations().end(); ++it) {
		// std::cout << *(*it);
		if ((*it)->getPath() == request.getTarget()) {
			location = *it;
			isRootLocation = true;
			break;
		}
	}
	if (location) {
		// std::cout << "ACHOU LOCATION\n" << "root:" << location->getRoot() << "\n";
		if (location->getRoot() != "")
			root = location->getRoot();
		if (!location->getRequestshttp().empty()) {
			this->_allowedMethods = location->getRequestshttp();
			// std::cout << "AQUI HTTP:\n";
			// for(std::set<std::string>::const_iterator it = this->_allowedMethods.begin(); it != this->_allowedMethods.end(); it++) {
			// 	std::cout << *it << "\n";
			// }
		}
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
	// Execute any relevant CGI scripts
	return (_applyMethodHTTP(request, server, root, isRootLocation));
}

void Response::_clearVariables(void){
	this->_bodyLength = 0;
	this->_body = "";
	this->_rawResponse = "";
	this->_status = std::pair<std::string, std::string>();
	this->_headers.clear() ;
	this->_allowedMethods.clear();
}

// -Functions
std::ostream &operator<<(std::ostream &out, Response const &in) {
	(void)in;
	return (out);
}

/*
	FOR CLIENTconst bool &isRootLocation
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