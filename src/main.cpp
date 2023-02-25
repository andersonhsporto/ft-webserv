#include "WebServer.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Request.hpp"
#include "Response.hpp"
#include "Utils.hpp"

int main(int argc, char *argv[]) {
	// std::cout << utils::getCurrentTime() << "\n";
    // {
	// 	WebServer	webserver;
	// 	if (argc > 1)
	// 		webserver.parse(argv[1]);
	// }
	// {
	// 	std::cout << "\n\n\tTest Request Parser\n";
	// 	Request		test;

	// 	if (argc > 2) {
	// 		std::ifstream		file;
	// 		std::string			fileString;
	// 		std::istringstream	iss;
	// 		fileString = "";
	// 		file.open(argv[2]);
	// 		if (!file.is_open()) {
	// 			throw std::runtime_error("Failed to open config file");
	// 		}
	// 		std::string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	// 		iss.str(fileContent);
	// 		test.parseRequest(fileContent);
	// 		std::cout << test;
	// 	}
	// }
	// {
	// 	std::cout << "\n\n\tTest Response Parser\n";

	// 	if (argc > 2) {
	// 		std::ifstream		file;
	// 		std::string			fileString;
	// 		std::istringstream	iss;
	// 		fileString = "";
	// 		file.open(argv[2]);
	// 		if (!file.is_open()) {
	// 			throw std::runtime_error("Failed to open config file");
	// 		}
	// 		std::string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	// 		iss.str(fileContent);
	// 		Response	test(fileContent);
	// 		std::cout << test;
	// 	}
	// }
	return (0);
}


/*
		TO TEST SOCKET
		CREATES A SERVER WITH SOCKET WAITING FOR CLIENT MESSAGE
		LIKE "MINITALK" PROJECT
*/
/*
#include "Socket.hpp"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cerrno>

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }

    int port = atoi(argv[1]);

    // Create the socket
    Socket socket;
    if (!socket.bind("0.0.0.0", port)) {
        std::cerr << "Failed to bind the socket to 0.0.0.0:" << port << std::endl;
        return 1;
    }
    if (!socket.listen(1)) {
        std::cerr << "Failed to listen on 0.0.0.0:" << port << std::endl;
        return 1;
    }

    // Wait for incoming connections
    std::cout << "Waiting for incoming connections on 0.0.0.0:" << port << std::endl;
    while (true) {
        const int BUFFER_SIZE = 1024;
        char buffer[BUFFER_SIZE];
        Socket client_socket = socket.accept();
        if (client_socket.getFd() == -1) {
            continue;
        }
        // Send a message to the client
        const char *message = "From Server: Hello, client!\n";
        if (client_socket.send(message, strlen(message), 0) == -1) {
            std::cerr << "Failed to send message to client" << std::endl;
            continue;
        }
        ssize_t received_bytes = client_socket.recv(buffer, BUFFER_SIZE, 0);
        if (received_bytes == -1) {
            std::cerr << "Error receiving data: " << strerror(errno) << std::endl;
            break;
        }
        std::cout << "Received message: " << buffer << std::endl;
        // Close the connection
		client_socket.close();
		memset(buffer, 0, BUFFER_SIZE);
    }
    return 0;
}
*/