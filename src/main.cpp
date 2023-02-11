#include "WebServer.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Request.hpp"
#include "Response.hpp"

int main(int argc, char *argv[]) {
	{
		WebServer	webserver;
		if (argc > 1)
			webserver.parse(argv[1]);
	}
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
	{
		std::cout << "\n\n\tTest Response Parser\n";

		if (argc > 2) {
			std::ifstream		file;
			std::string			fileString;
			std::istringstream	iss;
			fileString = "";
			file.open(argv[2]);
			if (!file.is_open()) {
				throw std::runtime_error("Failed to open config file");
			}
			std::string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
			iss.str(fileContent);
			Response	test(fileContent);
			std::cout << test;
		}
	}
	return (0);
}