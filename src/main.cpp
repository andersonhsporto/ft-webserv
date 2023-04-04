#include "WebServer.hpp"
#include <iostream>
#include <csignal>

WebServer webserver;

void sigIntHandlerFunc(int sig) {
	(void)sig;
	std::cout << "\nFinishing the WebServer...\n";
	webserver.finish();
	exit(0);
}

int main(int argc, char *argv[]) {
	{
		struct sigaction sigIntHandler;
		sigIntHandler.sa_handler = sigIntHandlerFunc;
  		sigemptyset(&sigIntHandler.sa_mask);
  		sigIntHandler.sa_flags = 0;
  		sigaction(SIGINT, &sigIntHandler, NULL);
		if (argc > 1){
			try{
				webserver.run(argv[1]);
			}catch(const std::exception &e){
				std::cout << e.what();
			}
		}
	}
	return (0);
}