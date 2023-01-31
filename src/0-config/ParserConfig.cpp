#include "ParserConfig.hpp"
#include "WebServer.hpp"

// -Constructors
// ParserConfig::ParserConfig(void) : _webserver() {
// 	this->_parseFuncs["listen"] = &_parseListen;
// 	this->_parseFuncs["server_name"] = &_parseServerName;
// 	this->_parseFuncs["client_max_body_size"] = &_parseMaxSizeBody;
// 	this->_parseFuncs["root"] = &_parseRoot;
// 	this->_parseFuncs["index"] = &_parseIndex;
// 	this->_parseFuncs["error_page"] = &_parseErrorPage;
// 	this->_parseFuncs["timeout"] = &_parseTimeOut;
// 	this->_parseFuncs["location"] = &_parseLocation;
// 	std::cout << "ParserConfig default constructor called\n";
// 	return ;
// }

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

// ParserConfig::ParserConfig(ParserConfig const &rhs) {
// 	std::cout << "ParserConfig copy constructor called\n";
// 	*this = rhs;
// 	return ;
// }

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
	// std::cout << "Here\n";
	// for (size_t i = 0; i < this->_configServers.size(); i++) {
	// 	std::cout << "Server " << i + 1 << ": " << this->_configServers[i] << std::endl;
	// }
}


inline void ParserConfig::_setServers() {
	std::cout << "Inside set Server\n";
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

			// std::cout << "Key: " << key << std::endl;
			// std::cout << "Value: " << value << std::endl;
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
	// std::cout << "Parser Listen called:   " << "The value: "<< value << "\n\n";
	// std::cout << "Server Number: " << server << "\n\n";
	in_addr_t	numbHost;
	int			numbPort;
	std::string	host;
	std::string	port;
	std::string::size_type pos = value.find(':');


	host = value.substr(0, pos);
	numbHost = inet_addr(host.c_str());
	port = value.substr(pos + 1);
	std::istringstream iss(port);
	iss >> numbPort;

	server.setHost(numbHost);
	server.setPort(numbPort);

	// debug
	// in_addr ip_address;
	// ip_address.s_addr = server.getHost();
	// char* string_ip = inet_ntoa(ip_address);
	// std::cout << "Binary host: " << server.getHost() << "\nString host: " << string_ip << "\nport: " << server.getPort() << "\n";
}

void ParserConfig::_parseServerName(const std::string &value, class Server &server) {
	// std::cout << "Parser Server Name called:   " << "The value: "<< value << "\n";
    // std::cout << "Server Number: " << server << "\n\n";
	std::vector<std::string> split;
	std::istringstream iss(value);
	std::string line;
	while (std::getline(iss, line, ' ')) {
		split.push_back(line);
	}
	server.setServername(split);
	// for (std::vector<std::string>::const_iterator it = server.getServername().begin(); it != server.getServername().end(); ++it) {
	// 	std::cout << "Name: " << *it << "\n";
	// }
}


void ParserConfig::_parseMaxSizeBody(const std::string &value, class Server &server) {
	// std::cout << "Parser Max Size Body called:   " << "The value: "<< value << "\n\n";
	// std::cout << "Server Number: " << server << "\n\n";
	int maxBodySize;

	std::istringstream iss(value);
	iss >> maxBodySize;
	server.setMaxbodysize(maxBodySize);
	// std::cout << "Size: " << server.getMaxbodysize() << "\n";
}


void ParserConfig::_parseRoot(const std::string &value, class Server &server) {
	// std::cout << "Parser Root called:   " << "The value: "<< value << "\n\n";
    // std::cout << "Server Number: " << server << "\n\n";
	
	server.setRoot(value);
	// std::cout << "Root: " << server.getRoot() << "\n";
}

void ParserConfig::_parseIndex(const std::string &value, class Server &server) {
	// std::cout << "Parser Index called:   " << "The value: "<< value << "\n\n";
    // std::cout << "Server Number: " << server << "\n\n";
	std::vector<std::string> split;
	std::istringstream iss(value);
	std::string line;

	while (std::getline(iss, line, ' ')) {
		split.push_back(line);
	}
	server.setIndex(split);
	// for (std::vector<std::string>::const_iterator it = server.getIndex().begin(); it != server.getIndex().end(); ++it) {
	// 	std::cout << "Name: " << *it << "\n";
	// }
}

void ParserConfig::_parseErrorPage(const std::string &value, class Server &server) {
	// std::cout << "Parser Error Page called:   " << "The value: "<< value << "\n\n";
    // std::cout << "Server Number: " << server << "\n\n";
	int code;
	std::string page;
	std::string::size_type pos = value.find(' ');

	std::istringstream iss(value.substr(0, pos));
	iss >> code;
	page = value.substr(pos + 1);
	server.addErrorPages(code, page);
	// std::cout << "Error code:" << code << " page: " << server.getErrorpages().at(code) << "\n";
}

void ParserConfig::_parseTimeOut(const std::string &value, class Server &server) {
	// std::cout << "Parser TimeOut called:   " << "The value: "<< value << "\n\n";
    // std::cout << "Server Number: " << server << "\n\n";
	int timeOut;

	std::istringstream iss(value);
	iss >> timeOut;
	server.setTimeout(timeOut);
	// std::cout << "Size: " << server.getTimeout() << "\n";
}

void ParserConfig::_parseLocation(const std::string &value, class Server &server) {
	// std::cout << "Parser Location called:   " << "The value: "<< value << "\n\n";
    // std::cout << "\nServer Number: " << server << "\n";
	server.addLocations(value);
	// std::cout << "Location Number: " << server.getLocations().back()->name << "\n";
}


// -Functions
std::ostream &operator<<(std::ostream &out, ParserConfig const &in) {
	out << "The file:\n" << in.getFile() << "\n";
	return (out);
}
