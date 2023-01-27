#include "ParserConfig.hpp"

int main(int argc, char *argv[]) {

	ParserConfig	Parser;
	if (argc == 2)
		Parser.parseFile(argv[1]);

	// std::cout << Parser.get_file();
	return 0;
}