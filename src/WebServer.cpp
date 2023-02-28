#include "WebServer.hpp"
#include "Poll.hpp"
#include "Request.hpp"
#include "Response.hpp"

// -Constructors
WebServer::WebServer(void) : _parser(ParserConfig(*this)) {
	std::cout << "WebServer default constructor called\n";
	return ;
}

// -Destructor
WebServer::~WebServer(void) {
	std::cout << "WebServer default destructor called\n";
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
void WebServer::run(const std::string &FilePath) {
	Poll					poller;
	std::vector<Socket *>	listSockets;

	this->_parser.parseFile(FilePath);
	Socket listener;
	Socket* ptr;
	for(std::vector<Server *>::iterator it = this->_serverList.begin(); it != this->_serverList.end(); ++it){
		listener = (*it)->getListener();
		listener.bind((*it)->getHost(), (*it)->getPort());
		listener.listen(SOMAXCONN);
		ptr = new Socket(listener); // create a temporary copy of the listener object
		listSockets.push_back(ptr);
	}
	poller.init(listSockets);
	while (true) {
		// Wait for incoming requests on the sockets using Poll
		poller.run();
		// Check for events on the listening socket
		for (size_t i = 0; i < poller.getSize(); i++) {
			std::cout << "Olaaaaaaaaaaaa\n";
			if (poller.checkEvent(poller.getEventReturn(i))) {
				// Handle incoming data on the socket
				Socket *current_socket = poller.getSocket(i);
				if (current_socket->isListener()) {
					// Accept a new connection on the listening socket
					Socket client_socket = current_socket->accept();
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

					std::string rawRequest;
					const int BUFFER_SIZE = 1024;
					char buffer[BUFFER_SIZE];
					ssize_t bytesRead;
					while ((bytesRead = current_socket->recv(buffer, sizeof(buffer), 0)) > 0) {
						rawRequest.append(buffer, bytesRead);

						// Check if we have received the entire request
						if (rawRequest.find("\r\n\r\n") != std::string::npos) {
							break;
						}
					}
					// Handle incoming data on the client socket
					Request request(rawRequest);
					Response response(*(current_socket->getServer()), request);
					// Send the Response object back to the client socket
					current_socket->send(response.getRawresponse().c_str(), response.getRawresponse().size(), 0);
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
