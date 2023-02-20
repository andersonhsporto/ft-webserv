#include "Server.hpp"
#include "Socket.hpp"
#include "Poll.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Utils.hpp"

// -Constructors
Server::Server(void) : _maxBodySize(1024), _timeOut(30), _port(80), _host(INADDR_ANY), _root("."), _running(false) {
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

void Server::start(void) {
	Socket	listener;
	Poll	poller;
	short	ret;

	if (!listener.bind(utils::intToString(_port), _host)) {
		// handle error
	}
	if (!listener.listen(SOMAXCONN)) {
		// handle error
	}
	std::vector<Socket *> socketArray;
	socketArray.push_back(&listener);
	poller.init(socketArray);
	poller.run();
	ret = poller.getEventReturn(0);
	if ((ret & POLLPRI) == POLLPRI)
		std::cout << "(PARTE1) retorno do status do pool: POLLPRI\n";
	if ((ret & POLLIN) == POLLIN)
		std::cout << "(PARTE1) retorno do status do pool: POLLIN\n";
	if ((ret & POLLOUT) == POLLOUT)
		std::cout << "(PARTE1) retorno do status do pool: POLLOUT\n";
	if ((ret & POLLWRBAND) == POLLWRBAND)
		std::cout << "(PARTE1) retorno do status do pool:POLLWRBAND\n";
	while (true) {
		// Wait for incoming requests on the sockets using Poll
		poller.run();

		// Check for events on the listening socket
		for (size_t i = 0; i < poller.getSize(); i++) {
			if (poller.getEventReturn(i) & POLLIN) {
				// Handle incoming data on the socket
				Socket *current_socket = poller.getSocket(i);

				if (current_socket == &listener) {
					// Accept a new connection on the listening socket
					Socket client_socket = listener.accept();
					if (client_socket.getFd() == -1) {
						std::cerr << "Failed to accept new connection" << std::endl;
						continue;
					}
					// Add the new client socket to the poller
					/*
						Tem q fazer algo assim, acho
					*/
					// poller.addSocket(&client_socket, POLLIN);
				} else {
					// Handle incoming data on the client socket
					/*
							Fazendo
						Request request(current_socket->recv());
						Response response(this, request);

						// Send the Response object back to the client socket
						current_socket->send(response.to_string());
					*/
				}
			}
			// check for other events and handle appropriately
			if (poller.getEventReturn(i) & POLLOUT) {
				// handle POLLOUT event
			}
			if (poller.getEventReturn(i) & POLLWRBAND) {
				// handle POLLWRBAND event
			}
			if (poller.getEventReturn(i) & POLLPRI) {
				// handle POLLPRI event
			}
		}
	}
}

// -Functions
std::ostream &operator<<(std::ostream &out, Server const &in) {
	(void)in;
	return (out);
}
