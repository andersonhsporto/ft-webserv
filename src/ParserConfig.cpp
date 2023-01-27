#include "ParserConfig.hpp"

// -Constructors
ParserConfig::ParserConfig(void) {
	std::cout << "ParserConfig default constructor called\n";
	return ;
}

ParserConfig::ParserConfig(ParserConfig const &rhs) {
	std::cout << "ParserConfig copy constructor called\n";
	*this = rhs;
	return ;
}

// -Destructor
ParserConfig::~ParserConfig(void) {
	std::cout << "ParserConfig default destructor called\n";
	return ;
}

// -Operators
ParserConfig &ParserConfig::operator=(ParserConfig const &rhs) {
	if (this != &rhs) {
		std::cout << "ParserConfig copy assignment operator called\n";
	}
	return (*this);
}

// -Getters
const std::string &ParserConfig::get_file(void) const {
	return (this->_file);
}

// -Setters
void ParserConfig::set_file(std::string file) {
	this->_file = file;
}

// -Methods

// Correct method void ParserConfig::parseFile(const std::string &FilePath, WebServer &WebServer)
void ParserConfig::parseFile(const std::string &FilePath) {
	this->openFile(FilePath);
	this->splitServers();
	return ;
}


// -Private Methods

inline void ParserConfig::splitServers() {
	std::stack<int> brackets;
	std::string currentServer;
	bool insideServer = false;

	for (size_t i = 0; i < this->get_file().size(); i++) {
		if (this->get_file()[i] == '{') {
			if (!insideServer) {
				currentServer += this->get_file()[i];
				insideServer = true;
			}
			brackets.push(i);
		}
		else if (this->get_file()[i] == '}') {
			if (brackets.size() == 1) {
				currentServer += this->get_file()[i];
				this->_configServers.push_back(currentServer);
				currentServer.clear();
				insideServer = false;
			}
			brackets.pop();
		}
		else if (insideServer) {
			currentServer += this->get_file()[i];
		}
	}
	if (!brackets.empty()) {
		throw std::runtime_error("Unclosed curly brace");
	}
	std::cout << "Here\n";
	for (size_t i = 0; i < this->_configServers.size(); i++) {
		std::cout << "Server " << i + 1 << ": " << this->_configServers[i] << std::endl;
	}
}

inline void	ParserConfig::openFile(const std::string &FilePath) {
	std::ifstream file;

	file.open(FilePath.c_str());
	if (!file.is_open()) {
		throw std::runtime_error("Failed to open config file");
	}
	std::string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	this->set_file(fileContent);
}

// -Functions
std::ostream &operator<<(std::ostream &out, ParserConfig const &in) {
	out << "The file:\n" << in.get_file() << "\n";
	return (out);
}
