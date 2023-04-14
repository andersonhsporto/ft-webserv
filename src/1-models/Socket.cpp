#include "Socket.hpp"
#include <iostream>
#include "Server.hpp"
#include "../../includes/0-config/Utils.hpp"
#include <fcntl.h>

// -Constructors
Socket::Socket(void) : _fd(-1) {
	std::cout << "Starting socket..." << std::endl;
	return ;
}

Socket::Socket(int fd) : _fd(fd) {
	if (fcntl(_fd, F_SETFL, O_NONBLOCK) < 0)
		throw std::runtime_error("ERROR(2): unable to set socket to non-blocking\n");
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
    if (_fd != -1)
	    std::cout << "Closing socket fd: " << _fd << std::endl;
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
bool Socket::bind() {
	int ok = 1;
	_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_fd == -1) {
		return false;
	}
	if(::setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &ok, sizeof(int)) < 0)
		throw std::runtime_error("ERROR(1): Could not set socket options\n");
	if (fcntl(_fd, F_SETFL, O_NONBLOCK) < 0)
		throw std::runtime_error("ERROR(1): unable to set socket to non-blocking\n");
	struct sockaddr_in addr = {};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(this->_server->getPort());
	addr.sin_addr.s_addr = inet_addr(this->_server->getHost().c_str());
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

int		Socket::recv(std::string &request){
	const int				BUFFER_SIZE = 1024;
	char					buffer[BUFFER_SIZE];
	int						bytes;

	do {
		bytes = ::recv(this->getFd(), buffer, BUFFER_SIZE, 0);
		request.append(buffer, bytes);
		if(request.find("Expect: 100-continue") != std::string::npos)
			sleep(2);
		if( utils::sizeBody(request) < utils::findContentLenght(request))
			continue;

		// Check if we have received the entire request
		if (request.find("\r\n\r\n") != std::string::npos || bytes == 0) {
			break;
		}
	}while (bytes > 0);
	return bytes;
}

int		Socket::send(const std::string response){
	int		bytes;
	
	for(int bytesSend = 0; (long unsigned int)bytesSend < response.size(); ) {
		bytes = ::send(this->getFd(), response.c_str(), response.size(), 0);
		if(bytes <= 0)
			break;
		bytesSend += bytes;
	}
	return bytes;
}

int Socket::accept(void) {
	struct sockaddr_in clientAddr = {};
	socklen_t clientAddrLen = sizeof(clientAddr);
	return (::accept(_fd, (struct sockaddr*)&clientAddr, &clientAddrLen));
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
