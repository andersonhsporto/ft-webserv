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
const std::string &ParserConfig::getFile(void) const {
	return (this->_file);
}

// -Setters
void ParserConfig::setFile(std::string _file) {
  this->_file = _file;
}

// -Methods

// Correct method void ParserConfig::parseFile(const std::string &FilePath, WebServer &WebServer)
void ParserConfig::parseFile(const std::string &FilePath) {
	this->_openFile(FilePath);
	this->_splitServers();
	return ;
}


// -Private Methods

inline void ParserConfig::_splitServers() {
	std::stack<int> brackets;
	std::string currentServer;
	bool insideServer = false;

	for (size_t i = 0; i < this->getFile().size(); i++) {
		if (this->getFile()[i] == '{') {
			if (!insideServer) {
				currentServer += this->getFile()[i];
				insideServer = true;
			}
			brackets.push(i);
		}
		else if (this->getFile()[i] == '}') {
			if (brackets.size() == 1) {
				currentServer += this->getFile()[i];
				this->_configServers.push_back(currentServer);
				currentServer.clear();
				insideServer = false;
			}
			brackets.pop();
		}
		else if (insideServer) {
			currentServer += this->getFile()[i];
		}
	}
	std::cout << "Here\n";
	for (size_t i = 0; i < this->_configServers.size(); i++) {
		std::cout << "Server " << i + 1 << ": " << this->_configServers[i] << std::endl;
	}
}

bool ParserConfig::_isCurlyBracketBalanced(std::string fileContent) {
  std::stack<char> stack;
  for (std::string::size_type i = 0; i < fileContent.size(); i++) {
    if (fileContent[i] == '{') {
      stack.push('{');
    } else if (fileContent[i] == '}') {
      if (stack.empty())
        return false;
      stack.pop();
    }
  }
  return stack.empty();
}


inline void	ParserConfig::_openFile(const std::string &FilePath) {
	std::ifstream file;

	file.open(FilePath.c_str());
	if (!file.is_open()) {
		throw std::runtime_error("Failed to open config file");
	}
	std::string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    if (!this->_isCurlyBracketBalanced(fileContent)) {
        throw std::runtime_error("Curly brackets are not balanced");
    }

	this->setFile(fileContent);
}

// -Functions
std::ostream &operator<<(std::ostream &out, ParserConfig const &in) {
	out << "The file:\n" << in.getFile() << "\n";
	return (out);
}
