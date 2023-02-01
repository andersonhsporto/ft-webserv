#include "WebServer.hpp"

int main(int argc, char *argv[]) {
	WebServer	webserver;

	if (argc == 2)
		webserver.parse(argv[1]);
	return (0);
}