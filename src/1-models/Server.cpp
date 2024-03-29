#include "Server.hpp"
#include "Socket.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Utils.hpp"

// -Constructors
Server::Server(void) : _maxBodySize(1024), _timeOut(30), _port(0), _autoindex(false), _host(""), _root(".") {
	std::cout << "Starting server..." << std::endl;
	return ;
}

Server::Server(Server const &rhs) {
	*this = rhs;
	return ;
}

// -Destructor
Server::~Server(void) {
    std::cout << "Closing server named: " << _serverName[0] << std::endl;
		for (std::vector<ServerLocation*>::iterator it = _locations.begin(); it != _locations.end(); ++it) {
			delete *it;
		}
	return ;
}

// -Operators
Server &Server::operator=(Server const &rhs) {
	if (this != &rhs) {
		std::cout << "Server copy assignment operator called\n";
	}
	return (*this);
}

// -Getters
const bool &Server::getAutoindex(void) const{
	return this->_autoindex;
}

const std::map<std::string,std::string> &Server::getCgi(void) const {
	return (this->_cgi);
}

const int &Server::getMaxbodysize(void) const {
	return (this->_maxBodySize);
}

const int &Server::getTimeout(void) const {
	return (this->_timeOut);
}

const std::map<int,std::string> &Server::getErrorpages(void) const {
	return (this->_errorPages);
}

const std::vector<ServerLocation*> &Server::getLocations(void) const {
	return (this->_locations);
}

const std::vector<std::string> &Server::getServername(void) const {
	return (this->_serverName);
}

const std::vector<std::string> &Server::getIndex(void) const {
	return (this->_index);
}

const std::string &Server::getRoot(void) const {
	return (this->_root);
}

const std::string &Server::getCGIFolder() const {
    return (this->_CGIFolder);
}

const int &Server::getPort(void) const {
	return (this->_port);
}

const std::string &Server::getHost(void) const {
	return (this->_host);
}

Socket Server::getListener(void) const{
	return (this->_listener);
}

// -Setters
void Server::setAutoindex(bool autoindex){
	this->_autoindex = autoindex;
}

void Server::setCgi(std::map<std::string,std::string> Cgi) {
	this->_cgi = Cgi;
}

void Server::setMaxbodysize(int Maxbodysize) {
	this->_maxBodySize = Maxbodysize;
}

void Server::setTimeout(int Timeout) {
	this->_timeOut = Timeout;
}

void Server::setErrorpages(std::map<int,std::string> Errorpages) {
	this->_errorPages = Errorpages;
}

void Server::setLocations(std::vector<ServerLocation*> Locations) {
	this->_locations = Locations;
}

void Server::setServername(std::vector<std::string> Servername) {
	this->_serverName = Servername;
}

void Server::setIndex(std::vector<std::string> Index) {
	this->_index = Index;
}

void Server::setRoot(std::string Root) {
	this->_root = Root;
}

void Server::setCGIFolder(std::string CGIFolder) {
    this->_CGIFolder = CGIFolder;
}

void Server::setPort(int Port) {
	this->_port = Port;
}

void Server::setHost(std::string Host) {
	this->_host = Host;
}

// -Methods
void Server::addErrorPages(const int &code, const std::string &page) {
	this->_errorPages[code] = page;
}

void Server::addLocations(const std::string &values) {
	ServerLocation *add = new ServerLocation(values);
	this->_locations.push_back(add);
}

void Server::addCgi(const std::string &extension, const std::string &path) {
	this->_cgi[extension] = path;
}

// -Functions
std::ostream &operator<<(std::ostream &out, Server const &in) {
	(void)in;
	return (out);
}
