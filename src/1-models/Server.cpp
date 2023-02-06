#include "Server.hpp"
// -Constructors
Server::Server(void) {
	std::cout << "Server default constructor called\n";
	return ;
}

Server::Server(Server const &rhs) {
	std::cout << "Server copy constructor called\n";
	*this = rhs;
	return ;
}

// -Destructor
Server::~Server(void) {
	std::cout << "Server default destructor called\n";
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

const int &Server::getPort(void) const {
	return (this->_port);
}

const in_addr_t &Server::getHost(void) const {
	return (this->_host);
}

// -Setters
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

void Server::setPort(int Port) {
	this->_port = Port;
}

void Server::setHost(in_addr_t Host) {
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
