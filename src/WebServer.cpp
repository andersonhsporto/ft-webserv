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
WebServer::~WebServer(void) {
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
	Socket					client_socket;
	std::string				rawRequest;
	const int				BUFFER_SIZE = 1024;
	char					buffer[BUFFER_SIZE];
	ssize_t					bytesRead;
	Request 				request;
	Response 				response;
	int                     client_fd;


	this->_parser.parseFile(FilePath);
	for(std::vector<Server *>::iterator it = this->_serverList.begin(); it != this->_serverList.end(); ++it){
		listener = new Socket();
		listener->setServer((*it));
		if(listener->bind((*it)->getHost(), (*it)->getPort())){
			if(listener->listen(SOMAXCONN)){
				this->_poller.addSocket(listener);
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
		for (size_t i = 0; i < this->_poller.getSize(); i++) {
			std::cout << "FD: " << this->_poller.getSocket(i)->getFd() << "\n";
			if (this->_poller.checkEvent(this->_poller.getEventReturn(i))) {
				// Handle incoming data on the socket
				listener = this->_poller.getSocket(i);
				// Accept a new connection on the listening socket
				client_fd = listener->accept();
				if (client_fd == -1) {
					std::cerr << "Failed to accept new connection" << std::endl;
					continue;
				}
				std::cout << "The FD above generated the client with the FD " << client_fd << "\n";
				rawRequest = "";
				bytesRead = 0;
				//Receive customer data 
				while ((bytesRead = ::recv(client_fd, buffer, sizeof(buffer), 0)) > 0) {
					rawRequest.append(buffer, bytesRead);

					// Check if we have received the entire request
					if (rawRequest.find("\r\n\r\n") != std::string::npos) {
						break;
					}
				}
				// Handle incoming data on the client socket
				request.execute(rawRequest);
				response.execute(*(listener->getServer()), request);
				// Send the Response object back to the client socket
				::send(client_fd, response.getRawresponse().c_str(), response.getRawresponse().size(), 0);
				::close(client_fd); 
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
