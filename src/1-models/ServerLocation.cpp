#include "ServerLocation.hpp"
#include <set>
#include "Utils.hpp"

// -Constructors
ServerLocation::ServerLocation(void) {
	this->_parseFuncs["/"] = &_parsePath;
	this->_parseFuncs["return"] = &_parseReturn;
	this->_parseFuncs["root"] = &_parseRoot;
	this->_parseFuncs["limit_except"] = &_parseRequest;
	this->_parseFuncs["autoindex"] = &_parseAutoIndex;
	std::cout << "ServerLocation default constructor called\n";
	return ;
}

ServerLocation::ServerLocation(const std::string &values) : _returnPage(std::make_pair(0, "")) {
	this->_parseFuncs["/"] = &_parsePath;
	this->_parseFuncs["return"] = &_parseReturn;
	this->_parseFuncs["root"] = &_parseRoot;
	this->_parseFuncs["limit_except"] = &_parseRequest;
	this->_parseFuncs["autoindex"] = &_parseAutoIndex;
	this->_parseValues(values);
	std::cout << "ServerLocation string constructor called\n";
	return ;
}

ServerLocation::ServerLocation(ServerLocation const &rhs) {
	std::cout << "ServerLocation copy constructor called\n";
	*this = rhs;
	return ;
}

// -Destructor
ServerLocation::~ServerLocation(void) {
	std::cout << "ServerLocation default destructor called\n";
	return ;
}

// -Operators
ServerLocation &ServerLocation::operator=(ServerLocation const &rhs) {
	if (this != &rhs) {
		std::cout << "ServerLocation copy assignment operator called\n";
	}
	return (*this);
}

// -Getters
const bool &ServerLocation::getAutoindex(void) const {
	return (this->_autoIndex);
}

const std::set<std::string> &ServerLocation::getRequestshttp(void) const {
	return (this->_requestsHttp);
}

const std::pair<int,std::string> &ServerLocation::getReturnpage(void) const {
	return (this->_returnPage);
}

const std::string &ServerLocation::getRoot(void) const {
	return (this->_root);
}

const std::string &ServerLocation::getPath(void) const {
	return (this->_path);
}

// -Setters
void ServerLocation::setAutoindex(bool Autoindex) {
	this->_autoIndex = Autoindex;
}

void ServerLocation::setRequestshttp(std::set<std::string> Requestshttp) {
	this->_requestsHttp = Requestshttp;
}

void ServerLocation::setReturnpage(std::pair<int,std::string> Returnpage) {
	this->_returnPage = Returnpage;
}

void ServerLocation::setRoot(std::string Root) {
	this->_root = Root;
}

void ServerLocation::setPath(std::string Path) {
	this->_path = Path;
}

// -Private Methods
void	ServerLocation::_parseValues(const std::string &values) {
	std::vector<std::string>	split;
	std::string					key;
	std::string					value;
	std::string					line;
	std::istringstream			ss;

	split = utils::splitStringBy(values, '\n');
	for (std::vector<std::string>::iterator it = split.begin(); it != split.end(); ++it) {
		line = *it;
		ss.str(line);
		ss >> key;
		utils::trimChar(key, ' ');
		if (key[0] == '/' && key.length() > 1) {
			value = key.substr(1);
			key = "/";
		}
		else {
			std::getline(ss, value);
			utils::trimChar(value, ' ');
		}
		try {
			this->_parseFuncs.at(key)(value, *this);
		} catch (const std::out_of_range &e) {
			throw std::runtime_error("Invalid server Location configuration");
		}
		ss.clear();
	}
}

void	ServerLocation::_parsePath(const std::string &value, ServerLocation &Location) {
	Location.setPath("/" + value);
}

void	ServerLocation::_parseReturn(const std::string &value, ServerLocation &Location) {
	std::string	code;
	std::string	address;

	utils::divideByDelimiter(value, code, address, ' ');
	Location.setReturnpage(std::make_pair(utils::stringToInt(code), address));
}

void	ServerLocation::_parseRoot(const std::string &value, ServerLocation &Location) {
	Location.setRoot(value);
}

void	ServerLocation::_parseRequest(const std::string &value, ServerLocation &Location) {
	std::vector<std::string>	split;
	std::set<std::string>		validHttp;

	split = utils::splitStringBy(value, ' ');
	validHttp.insert(split.begin(), split.end());
	Location.setRequestshttp(validHttp);
}

void	ServerLocation::_parseAutoIndex(const std::string &value, ServerLocation &Location) {
	if (value == "on")
		Location.setAutoindex(true);
}

// -Functions
std::ostream &operator<<(std::ostream &out, ServerLocation const &in) {
	out << "The Path: " << in.getPath() << "\n"
		"The Return Page: " << in.getReturnpage().first << " " << in.getReturnpage().second << "\n"
		"The Root: " << in.getRoot() << "\n"
		"AutoIndex is: " << std::boolalpha << in.getAutoindex() << "\n";
	for (std::set<std::string>::const_iterator it = in.getRequestshttp().begin(); it != in.getRequestshttp().end(); ++it) {
		out << "Request Http: " << *it << "\n";
	}
	return (out);
}
