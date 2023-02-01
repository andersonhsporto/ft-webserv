#include "ParserConfig.hpp"
#include "WebServer.hpp"
#include "ParserUtils.hpp"

ParserConfig::ParserConfig(WebServer &webserver) : _webServer(webserver) {
	this->_parseFuncs["listen"] = &_parseListen;
	this->_parseFuncs["server_name"] = &_parseServerName;
	this->_parseFuncs["client_max_body_size"] = &_parseMaxSizeBody;
	this->_parseFuncs["root"] = &_parseRoot;
	this->_parseFuncs["index"] = &_parseIndex;
	this->_parseFuncs["error_page"] = &_parseErrorPage;
	this->_parseFuncs["timeout"] = &_parseTimeOut;
	this->_parseFuncs["location"] = &_parseLocation;
	std::cout << "ParserConfig WebServer constructor called\n";
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
void ParserConfig::setFile(std::string file) {
	this->_file = file;
}

// -Methods
void ParserConfig::parseFile(const std::string &FilePath) {
	this->_openFile(FilePath);
	this->_splitServers();
	this->_setServers();
	return ;
}

// -Private Methods
inline void	ParserConfig::_openFile(const std::string &FilePath) {
	std::ifstream	file;
	std::string		fileString = "";

	file.open(FilePath.c_str());
	if (!file.is_open()) {
		throw std::runtime_error("Failed to open config file");
	}
	std::string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	std::istringstream iss(fileContent);
	for (std::string line; std::getline(iss, line); ) {
		if (!line.empty()) {
			fileString += line + '\n';
		}
	}

	if (!this->_isCurlyBracketBalanced(fileString)) {
		throw std::runtime_error("Curly brackets are not balanced");
	}
	this->setFile(fileString);
}

bool ParserConfig::_isCurlyBracketBalanced(std::string fileContent) {
	std::stack<char> stack;

	for (std::string::size_type i = 0; i < fileContent.size(); i++) {
		if (fileContent[i] == '{') {
			stack.push('{');
		}
		else if (fileContent[i] == '}') {
			if (stack.empty())
				return false;
			stack.pop();
		}
	}
	return stack.empty();
}

inline void ParserConfig::_splitServers() {
	std::stack<int> brackets;
	std::string currentServer;
	bool insideServer = false;

	for (size_t i = 0; i < this->getFile().size(); i++) {
		if (this->getFile()[i] == '{') {
			currentServer += this->getFile()[i];
			if (!insideServer) {
				insideServer = true;
			}
			brackets.push(i);
		}
		else if (this->getFile()[i] == '}') {
			currentServer += this->getFile()[i];
			if (brackets.size() == 1) {
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
}

inline void ParserConfig::_setServers() {
	std::vector<std::string> lines;
	
	for (std::vector<std::string>::iterator it = this->_configServers.begin(); it != this->_configServers.end(); ++it) {
		std::string line = *it;
		std::istringstream iss(line);

		for (std::string subLine; std::getline(iss, subLine, '\n');) {
			if (subLine.size() > 0 && subLine[subLine.size()-1] == ';') {
				subLine.resize(subLine.size()-1);
			}
			lines.push_back(subLine);
		}
		for (std::vector<std::string>::iterator it2 = lines.begin(); it2 != lines.end(); ++it2) {
			std::string &line = *it2;

			std::string key;
			std::string value;
			std::stringstream ss(line);

			ss >> key;
			std::size_t first = key.find_first_not_of(" ");
			std::size_t last = key.find_last_not_of(" ") + 1;
			if (first != std::string::npos && last != std::string::npos && first < last) {
				key = key.substr(first, last - first);
			}
			if (key == "{")
				this->_webServer.addServer();
			std::getline(ss, value);
			first = value.find_first_not_of(" ");
			last = value.find_last_not_of(" ") + 1;
			if (first != std::string::npos && last != std::string::npos && first < last) {
				if (key == "location")
					last--;
				value = value.substr(first, last - first);
			}
			if (key == "location") {
				while (it2 + 1 != lines.end() && (*(it2 + 1)).find("}") == std::string::npos) {
					it2++;
					value += "\n" + (*it2);
				}
				it2++;
			}
			if (key != "{" && key != "}")
			{
				try {
					this->_parseFuncs.at(key);
					_parseFuncs[key](value, this->_webServer.getLastServer());
				} catch (const std::out_of_range& e) {
					throw std::runtime_error("Invalid server configuration");
				}
			}
		}
		lines.clear();
	}
	return ;
}

void ParserConfig::_parseListen(const std::string &value, class Server &server) {
	std::string				host;
	std::string				port;

	parser::divideByDelimiter(value, host, port, ' ');
	server.setHost(inet_addr(host.c_str()));
	server.setPort(parser::stringToInt(port));
}

void ParserConfig::_parseServerName(const std::string &value, class Server &server) {
	server.setServername(parser::splitStringBy(value, ' '));
}

void ParserConfig::_parseMaxSizeBody(const std::string &value, class Server &server) {
	server.setMaxbodysize(parser::stringToInt(value));
}

void ParserConfig::_parseRoot(const std::string &value, class Server &server) {
	server.setRoot(value);
}

void ParserConfig::_parseIndex(const std::string &value, class Server &server) {
	server.setIndex(parser::splitStringBy(value, ' '));
}

void ParserConfig::_parseErrorPage(const std::string &value, class Server &server) {
	std::string	code;
	std::string	page;

	parser::divideByDelimiter(value, code, page, ' ');
	server.addErrorPages(parser::stringToInt(code), page);
}

void ParserConfig::_parseTimeOut(const std::string &value, class Server &server) {
	server.setTimeout(parser::stringToInt(value));
}

void ParserConfig::_parseLocation(const std::string &value, class Server &server) {
	server.addLocations(value);
}

// -Functions
std::ostream &operator<<(std::ostream &out, ParserConfig const &in) {
	out << "The file:\n" << in.getFile() << "\n";
	return (out);
}
