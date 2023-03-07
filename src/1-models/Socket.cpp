#include "Socket.hpp"
#include <iostream>
#include "Server.hpp"

// -Constructors
Socket::Socket(void) : _fd(-1) {
	std::cout << "Socket default constructor called\n";
	return ;
}

Socket::Socket(int fd) : _fd(fd) {
	std::cout << "Socket fd constructor called\n";
	return ;
}

Socket::Socket(Socket const &rhs) {
	std::cout << "Socket copy constructor called\n";
	*this = rhs;
	return ;
}

// -Destructor
Socket::~Socket(void) {
	std::cout << "Socket default destructor called\n";
	close();
	return ;
}

// -Operators
Socket &Socket::operator=(Socket const &rhs) {
	if (this != &rhs) {
		std::cout << "Socket copy assignment operator called\n";
		_fd = rhs._fd;
		_server = rhs._server;
	}
	return (*this);
}

// -Getters
const int &Socket::getFd(void) const {
	return (this->_fd);
}

Server *Socket::getServer(void) const {
	return (this->_server);
}

// -Setters

void Socket::setServer(Server *server ){
	this->_server = server;
}


// -Methods
bool Socket::bind(const std::string& address, uint16_t port) {
	_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_fd == -1) {
		return false;
	}
	struct sockaddr_in addr = {};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;
	std::cout << "Address: " << address << " Port: " << port << "\n";
	if (::bind(_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
		close();
		return false;
	}
	return (true);
}


bool Socket::listen(int backlog) {
	if (::listen(_fd, backlog) == -1) {
		close();
		return false;
	}
	return (true);
}

int Socket::accept(void) {
	struct sockaddr_in clientAddr = {};
	socklen_t clientAddrLen = sizeof(clientAddr);
	return (::accept(_fd, (struct sockaddr*)&clientAddr, &clientAddrLen));
}

void Socket::connect(const std::string& address, uint16_t port) {
	_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_fd == -1) {
		throw std::runtime_error("Failed to create socket");
	}
	struct sockaddr_in addr = {};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(address.c_str());
	if (::connect(_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
		close();
		throw std::runtime_error("Failed to connect to server");
	}
}

ssize_t Socket::send(const void* buf, size_t len, int flags) {
	return (::send(_fd, buf, len, flags));
}

ssize_t Socket::recv(char* buf, size_t len, int flags) {
	return (::recv(_fd, buf, len, flags));
}

void Socket::setsockopt(int level, int optname, const void* optval, socklen_t optlen) {
	if (::setsockopt(_fd, level, optname, optval, optlen) == -1) {
		throw std::runtime_error("Failed to set socket option");
	}
}

void Socket::close(void) {
	if (_fd != -1) {
		::close(_fd);
		_fd = -1;
	}
}

std::ostream &operator<<(std::ostream &out, Socket const &in) {
	(void)in;
	return (out);
}
