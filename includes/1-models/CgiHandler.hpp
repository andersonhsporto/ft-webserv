#ifndef FT_WEBSERVER_HPP
#define FT_WEBSERVER_HPP

#include <string>
#include <iostream>
#include <map>
#include <cstdio>

#include <sys/wait.h> // waitpid
#include <unistd.h> // fork, execve
#include <cstring> // strerror

#include "Server.hpp"
#include "TypeHelper.hpp"
#include "0-config/Request.hpp"

#define GREEN "\033[1;32m"
#define RED "\033[0;31m"
#define RESET "\033[0m"

class CgiHandler {
public:
    CgiHandler(const Server &server, const Request &request);

    CgiHandler(CgiHandler const &rhs);

    ~CgiHandler();

    CgiHandler &operator=(CgiHandler const &rhs);

    std::string startCgiHandler();

    void printMessage();

private:
    CgiHandler();

    Request _request;

    TypeHelper _typeHelper;

    std::map<std::string, std::string> _env;

    char **_envArray;

    char **_getEnvironmentVariables();

    void _initEnvironmentVariables();

    std::string toString(std::string::size_type i);

    bool _isPythonScript();

    std::string _handlePythonScript();

    std::string _handleBinaryScript();


};

std::ostream &operator<<(std::ostream &out, CgiHandler const &in);

#endif //FT_WEBSERVER_HPP
