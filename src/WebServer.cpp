#include "WebServer.hpp"
#include "Poll.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include <cstdlib>
#include <cstring>
// #include <vector>
// #include <cerrno>

// -Constructors
WebServer::WebServer(void) : _parser(ParserConfig(*this)) {
	std::cout << "WebServer default constructor called\n";
	return ;
}

// -Destructor
WebServer::~WebServer() {
	std::cout << "WebServer default destructor called\n";
	this->_poller.clear();
	return ;
}

// -Operators
WebServer &WebServer::operator=(WebServer const &rhs) {
	if (this != &rhs) {
		std::cout << "WebServer copy assignment operator called\n";
	}
	return (*this);
}

// -Getters
// -Setters
// -Methods
void WebServer::finish(void){
	_poller.clear();
	while(_serverList.size()){
		delete _serverList[_serverList.size() - 1];
		_serverList.pop_back();
	}
}
void WebServer::run(const std::string &FilePath) {
	Socket					*listener;
	Socket					*client_socket;
	std::string				rawRequest;
	const int				BUFFER_SIZE = 1024;
	char					buffer[BUFFER_SIZE];
	ssize_t					bytes;
	unsigned int			countListeners = 0;

	this->_parser.parseFile(FilePath);
	for(std::vector<Server *>::iterator it = this->_serverList.begin(); it != this->_serverList.end(); ++it){
		listener = new Socket();
		listener->setServer((*it));
		if(listener->bind((*it)->getHost(), (*it)->getPort())){
			if(listener->listen(SOMAXCONN)){
				this->_poller.addSocket(listener);
				++countListeners;
				continue;
			}
		}
		delete listener;
		this->finish();
		throw std::runtime_error("Error: could not create socket listener\n");
	}
	this->_poller.init();
	std::cout << "number of listener Sockets: " << this->_poller.getSize() << "\n\n";
	while (true) {
		// Wait for incoming requests on the sockets using Poll
		this->_poller.run();
		// Check for events on the listening socket
		for (int i = this->_poller.getSize() - 1; i >= 0 ; i--) {
			std::cout << "FD: " << this->_poller.getSocket(i)->getFd() << "\n";
			if (this->_poller.checkEvent(this->_poller.getEventReturn(i))) {
				// Handle incoming data on the socket
				if(i < countListeners){
					listener = this->_poller.getSocket(i);
					std::cout << "Accepting new client connection through FD " << listener->getFd() << "\n"; 
					client_socket = new Socket(listener->accept());
					client_socket->setServer(listener->getServer());
					if (client_socket->getFd() == -1) {
						std::cerr << "Failed to accept new connection" << std::endl;
						continue;
					}
					this->_poller.addSocket(client_socket);
					std::cout << "The FD above generated the client with the FD " << client_socket->getFd() << "\n";
				} else {
					client_socket = this->_poller.getSocket(i);
					std::cout << "Attending customer request with fd " << client_socket->getFd() << "\n";
					rawRequest.clear();
					//Receive customer data 
					while ((bytes = ::recv(client_socket->getFd(), buffer, sizeof(buffer), 0)) > 0) {
						rawRequest.append(buffer, bytes);

						// Check if we have received the entire request
						if (rawRequest.find("\r\n\r\n") != std::string::npos) {
							break;
						}
					}
					if(bytes < 0){
						std::cout << "Error: unable to receive data from client FD " << client_socket->getFd() << "\n";
						continue;
					}
					// Handle incoming data on the client socket
					Request request(rawRequest);
					Response response(*(listener->getServer()), request);
					// Send the Response object back to the client socket
					for(int bytesSend = 0; bytesSend < response.getRawresponse().size(); ){
						bytes = ::send(client_socket->getFd(), response.getRawresponse().c_str(), \
								response.getRawresponse().size(), 0);
						if(bytes < 0){
							std::cout << "Error: unable to send data to client FD " << client_socket->getFd() << "\n";
							break;
						}
						bytesSend += bytes;
					}
					::close(client_socket->getFd());
					_poller.deleteSocket(client_socket);
				} 
			}
		}
	}
}

void WebServer::addServer() {
	Server *add = new Server();
	this->_serverList.push_back(add);
}

Server &WebServer::getLastServer() {
	return (*this->_serverList.back());
}

// -Functions
std::ostream &operator<<(std::ostream &out, WebServer const &in) {
	(void)in;
	return (out);
}
