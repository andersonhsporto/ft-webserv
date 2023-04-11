#include "TypeHelper.hpp"

TypeHelper::TypeHelper() {}

TypeHelper::TypeHelper(const Server &server) {
    _port = toString(server.getPort());
    _path = server.getLocations()[0]->getPath();
    _cgiFolder = server.getCGIFolder();
    _cgi = server.getCgi();
}

TypeHelper::~TypeHelper() {

}

void TypeHelper::print() {
    std::cout << ORANGE << "TypeHelper" << std::endl;
    std::cout << "port: " << _port << std::endl;
    std::cout << "path: " << _path << std::endl;
    std::cout << "cgiPath: " << _cgiPath << std::endl;
    std::cout << "cgiFolder: " << _cgiFolder << std::endl;
    std::cout << "cgi: " << std::endl;
    for (std::map<std::string, std::string>::iterator it = _cgi.begin(); it != _cgi.end(); ++it) {
        std::cout << it->first << " => " << it->second << std::endl;
    }
    std::cout << RESET << std::endl;
}

const std::string &TypeHelper::getPort() const {
    return _port;
}

const std::string &TypeHelper::getCgiFolder() const {
    return _cgiFolder;
}

const std::map<std::string, std::string> &TypeHelper::getCgi() const {
    return _cgi;
}

const std::string &TypeHelper::getPath() const {
    return _path;
}

std::string TypeHelper::toString(std::string::size_type i) {
    std::stringstream ss;
    ss << i;
    return ss.str();
}

const std::string &TypeHelper::getBody() const {
    return _body;
}
