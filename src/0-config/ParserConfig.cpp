#include "ParserConfig.hpp"
#include "WebServer.hpp"
#include "Utils.hpp"

ParserConfig::ParserConfig(WebServer &webserver) : _webServer(webserver) {
	this->_parseFuncs["listen"] = &_parseListen;
	this->_parseFuncs["server_name"] = &_parseServerName;
	this->_parseFuncs["client_max_body_size"] = &_parseMaxSizeBody;
	this->_parseFuncs["root"] = &_parseRoot;
	this->_parseFuncs["index"] = &_parseIndex;
	this->_parseFuncs["error_page"] = &_parseErrorPage;
	this->_parseFuncs["timeout"] = &_parseTimeOut;
	this->_parseFuncs["location"] = &_parseLocation;
	this->_parseFuncs["cgi"] = &_parseCgi;
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
	std::string contentsConfig;

	if (utils::fileToString(FilePath, contentsConfig) != -1)
		contentsConfig = utils::removeComments(contentsConfig);
	/*else
		Error oppening file
	*/

	if(!this->_isCurlyBracketBalanced(contentsConfig))
		throw std::runtime_error("Curly brackets are not balanced in the file " + FilePath);
	this->setFile(contentsConfig);
	this->_splitServers();
	this->_setServers();
	return ;
}

// -Private Methods
bool ParserConfig::_isCurlyBracketBalanced(std::string fileContent) {
	std::stack<char>	stack;

	for (std::string::size_type i = 0; i < fileContent.size(); i++) {
		if (fileContent[i] == '{') {
			stack.push('{');
		}
		else if (fileContent[i] == '}') {
			if (stack.empty())
				return (false);
			stack.pop();
		}
	}
	return (stack.empty());
}

inline void ParserConfig::_splitServers() {
	std::stack<int>	brackets;
	std::string		currentServer;
	bool			insideServer;

	insideServer = false;
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
	return ;
}

inline void ParserConfig::_setServers() {
	std::vector<std::string>	lines;
	std::string					key;
	std::string					value;
	std::string					line;
	std::string					line2;
	std::istringstream			iss;
	std::istringstream			ss;

	for (std::vector<std::string>::iterator it = this->_configServers.begin(); it != this->_configServers.end(); ++it) {
		line = *it;
		iss.str(line);
		for (std::string subLine; std::getline(iss, subLine, '\n');) {
			if (subLine.size() > 0 && subLine[subLine.size()-1] == ';') {
				subLine.resize(subLine.size()-1);
			}
			lines.push_back(subLine);
		}
		for (std::vector<std::string>::iterator it2 = lines.begin(); it2 != lines.end(); ++it2) {
			line2 = *it2;
			ss.str(line2);
			ss >> key;
			utils::trimChar(key, ' ');
			if (key == "}" || key == "{") {
				if (key == "{")
					this->_webServer.addServer();
				ss.clear();
				continue;
			}
			std::getline(ss, value);
			utils::trimChar(value, ' ');
			if (key == "location") {
				while (it2 + 1 != lines.end() && (*(it2 + 1)).find("}") == std::string::npos) {
					it2++;
					value += "\n" + (*it2);
				}
				it2++;
				utils::removeSubstring(value, "location ");
			}
			try {
				this->_parseFuncs.at(key);
				_parseFuncs[key](value, this->_webServer.getLastServer());
			} catch (const std::out_of_range& e) {
				throw std::runtime_error("Invalid server configuration");
			}
			ss.clear();
		}
		lines.clear();
		iss.clear();
	}
	return ;
}

void ParserConfig::_parseListen(const std::string &value, class Server &server) {
	std::string				host;
	std::string				port;

	utils::divideByDelimiter(value, host, port, ' ');
	server.setHost(inet_addr(host.c_str()));
	server.setPort(utils::stringToInt(port));
}

void ParserConfig::_parseServerName(const std::string &value, class Server &server) {
	server.setServername(utils::splitStringBy(value, ' '));
}

void ParserConfig::_parseMaxSizeBody(const std::string &value, class Server &server) {
	server.setMaxbodysize(utils::stringToInt(value));
}

void ParserConfig::_parseRoot(const std::string &value, class Server &server) {
	server.setRoot(value);
}

void ParserConfig::_parseIndex(const std::string &value, class Server &server) {
	server.setIndex(utils::splitStringBy(value, ' '));
}

void ParserConfig::_parseErrorPage(const std::string &value, class Server &server) {
	std::string	code;
	std::string	page;

	utils::divideByDelimiter(value, code, page, ' ');
	server.addErrorPages(utils::stringToInt(code), page);
}

void ParserConfig::_parseTimeOut(const std::string &value, class Server &server) {
	server.setTimeout(utils::stringToInt(value));
}

void ParserConfig::_parseLocation(const std::string &value, class Server &server) {
	server.addLocations(value);
}

void ParserConfig::_parseCgi(const std::string &value, class Server &server) {
	std::string	extension;
	std::string	path;

	utils::divideByDelimiter(value, extension, path, ' ');
	server.addCgi(extension, path);
}

// -Functions
std::ostream &operator<<(std::ostream &out, ParserConfig const &in) {
	out << "The file:\n" << in.getFile() << "\n";
	return (out);
}
