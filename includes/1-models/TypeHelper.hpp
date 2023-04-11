#ifndef FT_WEBSERVER_SRC_1_MODELS_TYPEHELPER_HPP_
#define FT_WEBSERVER_SRC_1_MODELS_TYPEHELPER_HPP_

#include <string>
#include <iostream>
#include <map>
#include "Server.hpp"

#define ORANGE "\033[0;33m"
#define GREEN "\033[1;32m"
#define RESET "\033[0m"

class TypeHelper {
public:
    TypeHelper();

    TypeHelper(Server const &server);

    ~TypeHelper();

    void print();

    const std::string &getPort() const;
    const std::string &getPath() const;
    const std::string &getCgiPath() const;
    const std::string &getCgiFolder() const;
    const std::map<std::string, std::string> &getCgi() const;

    const std::string &getBody() const;

private:

    std::string toString(std::string::size_type i);

    std::string _port;
    std::string _path;
    std::string _cgiPath;
    std::string _cgiFolder;
    std::map<std::string, std::string>	_cgi;
    std::string _body;
};

#endif //FT_WEBSERVER_SRC_1_MODELS_TYPEHELPER_HPP_
