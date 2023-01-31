#include "ServerLocation.hpp"

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

ServerLocation::ServerLocation(int name, const std::string &values) : name(name) {
	this->_parseFuncs["/"] = &_parsePath;
	this->_parseFuncs["return"] = &_parseReturn;
	this->_parseFuncs["root"] = &_parseRoot;
	this->_parseFuncs["limit_except"] = &_parseRequest;
	this->_parseFuncs["autoindex"] = &_parseAutoIndex;
	this->_parseValues(values);
	// std::cout << "ServerLocation int constructor called\n";
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

const std::vector<std::string> &ServerLocation::getRequestshttp(void) const {
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

void ServerLocation::setRequestshttp(std::vector<std::string> Requestshttp) {
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

// -Methods
// -Private Methods
void	ServerLocation::_parseValues(const std::string &values) {
	std::vector<std::string> split;
	std::istringstream iss(values);
	std::string line;
	while (std::getline(iss, line, '\n')) {
		split.push_back(line);
	}
	
	for (std::vector<std::string>::iterator it = split.begin(); it != split.end(); ++it) {
		std::string &line = *it;
		std::string key;
		std::string value;
		std::stringstream ss(line);

		ss >> key;
		std::size_t first = key.find_first_not_of(" ");
		std::size_t last = key.find_last_not_of(" ") + 1;
		if (first != std::string::npos && last != std::string::npos && first < last) {
			key = key.substr(first, last - first);
		}
		if (key.at(0) == '/' && key.length() > 1) {
			value = key.substr(1);
			key = "/";
		}
		else {
			std::getline(ss, value);
			first = value.find_first_not_of(" ");
			last = value.find_last_not_of(" ") + 1;
			if (first != std::string::npos && last != std::string::npos && first < last) {
				value = value.substr(first, last - first);
			}
		}
		// std::cout << "Key: " << key << std::endl;
		// std::cout << "Value: " << value << std::endl;
		try {
			this->_parseFuncs.at(key);
			_parseFuncs[key](value, *this);
		} catch (const std::out_of_range& e) {
			throw std::runtime_error("Invalid server Location configuration");
		}
	}
}

void	ServerLocation::_parsePath(const std::string &value, ServerLocation &Location) {
	Location.setPath("/" + value);
	// std::cout << "THe Path: " << Location.getPath() << "\n";
}

void	ServerLocation::_parseReturn(const std::string &value, ServerLocation &Location) {
	int			numbCode;
	std::string	code;
	std::string	address;
	std::string::size_type pos = value.find(' ');


	code = value.substr(0, pos);
	address = value.substr(pos + 1);
	std::istringstream iss(code);
	iss >> numbCode;

	Location.setReturnpage(std::make_pair(numbCode,address));
	// std::cout << "The Return Page: " << Location.getReturnpage().first << " " << Location.getReturnpage().second << "\n";
}

void	ServerLocation::_parseRoot(const std::string &value, ServerLocation &Location) {
	Location.setRoot(value);
	// std::cout << "The Root: " << Location.getRoot() << "\n";
}

void	ServerLocation::_parseRequest(const std::string &value, ServerLocation &Location) {
	std::vector<std::string> split;
	std::istringstream iss(value);
	std::string line;

	while (std::getline(iss, line, ' ')) {
		split.push_back(line);
	}
	Location.setRequestshttp(split);
	// for (std::vector<std::string>::const_iterator it = Location.getRequestshttp().begin(); it != Location.getRequestshttp().end(); ++it) {
	// 	std::cout << "Request Http: " << *it << "\n";
	// }
}

void	ServerLocation::_parseAutoIndex(const std::string &value, ServerLocation &Location) {
	if (value == "on")
		Location.setAutoindex(true);
	// std::cout << "AutoIndex is: " << std::boolalpha << Location.getAutoindex() << "\n";
}

// -Functions
std::ostream &operator<<(std::ostream &out, ServerLocation const &in) {
	out << "Location number: " << in.name << "\n"
		"The Path: " << in.getPath() << "\n"
		"The Return Page: " << in.getReturnpage().first << " " << in.getReturnpage().second << "\n"
		"The Root: " << in.getRoot() << "\n"
		"AutoIndex is: " << std::boolalpha << in.getAutoindex() << "\n";
	for (std::vector<std::string>::const_iterator it = in.getRequestshttp().begin(); it != in.getRequestshttp().end(); ++it) {
		out << "Request Http: " << *it << "\n";
	}
	return (out);
}
