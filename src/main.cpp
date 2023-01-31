#include "WebServer.hpp"

int main(int argc, char *argv[]) {
	WebServer	webserver;

	// ParserConfig	Parser;
	if (argc == 2)
		webserver.parse(argv[1]);
	// 	Parser.parseFile(argv[1]);

	// std::cout << Parser.get_file();
	return 0;
}