#include "CgiHandler.hpp"

CgiHandler::CgiHandler() {}

CgiHandler::CgiHandler(const Server &server, const Request &request)
        : _request(request), _typeHelper(server) {

    _initEnvironmentVariables();
    this->_envArray = _getEnvironmentVariables();
}

CgiHandler::CgiHandler(const CgiHandler &rhs) {
    *this = rhs;
}

CgiHandler::~CgiHandler() {
    for (int i = 0; _envArray[i]; ++i) {
        delete[] _envArray[i];
    }
    delete[] _envArray;
}

CgiHandler &CgiHandler::operator=(const CgiHandler &rhs) {
    if (this != &rhs) {
        _request = rhs._request;
    }
    return *this;
}

std::string CgiHandler::startCgiHandler(std::string &response) {
    if (_isPythonScript()) {
        return _handlePythonScript(response);
    }
    return _handleBinaryScript(response);
}

bool CgiHandler::_isPythonScript() {
    return (_typeHelper.getCgi().begin()->first == ".py");
}

void CgiHandler::_closeAll(int fdInput, int fdOutput, int saveStdin, int saveStdout, FILE *fileInput, FILE *fileOutput) {
    dup2(saveStdin, STDIN_FILENO);
    dup2(saveStdout, STDOUT_FILENO);
    close(saveStdin);
    close(saveStdout);
    close(fdInput);
    close(fdOutput);
    fclose(fileInput);
    fclose(fileOutput);
}

std::string CgiHandler::_handleBinaryScript(std::string &response) {
    pid_t pid;
    std::string newBody;

    // salva o file descriptor do stdin e do stdout
    int saveStdin = dup(STDIN_FILENO);
    int saveStdout = dup(STDOUT_FILENO);

    // cria um arquivo temporario para o input e outro para o output
    FILE *fileInput = tmpfile();
    FILE *fileOutput = tmpfile();

    // pega o file descriptor do arquivo temporario
    long fdInput = fileno(fileInput);
    long fdOutput = fileno(fileOutput);

    // escreve o body da request no arquivo temporario
    write(fdInput, _request.getBody().c_str(), _request.getBody().size());
    // volta o ponteiro para o inicio do arquivo
    lseek(fdInput, 0, SEEK_SET);


    pid = fork();
    if (pid == -1) {
        std::cout << RED << "Error: unable to fork" << RESET << std::endl;
        return ("");
    } else if (!pid) {
        // redireciona o input e o output para os arquivos temporarios
        dup2(fdInput, STDIN_FILENO);
        dup2(fdOutput, STDOUT_FILENO);

        char *args[] = {NULL};

        // executa o binario
        execve(_typeHelper.getCgiFolder().c_str(), args, _envArray);
        write(STDOUT_FILENO, "Content-Type: text/html\r\n\r Status: 500 Internal Server Error\r\n\r)", 65);
    } else {
        clock_t start = clock();
        int status;
        while (true) {
            double total = (double) (clock() - start) / CLOCKS_PER_SEC;
            if (total > SECONDS) {
                // Verifica se o processo filho entrou em loop infinito
                if (waitpid(pid, &status, WNOHANG) == 0) {
                    _closeAll(fdInput, fdOutput, saveStdin, saveStdout, fileInput, fileOutput);
                    kill(pid, SIGSEGV);
                    return ("508");
                }
                kill(pid, SIGSEGV);
                break;
            }
            // verifica se o processo filho terminou antes dos 5 segundos
            if (waitpid(pid, &status, WNOHANG) == pid) {
                break;
            }
        }
        // Verifica se o processo filho retornou um erro
        if (WEXITSTATUS(status) != 0) {
            _closeAll(fdInput, fdOutput, saveStdin, saveStdout, fileInput, fileOutput);
            kill(pid, SIGSEGV);
            return ("500");
        }

        char buffer[2048] = {0};
        waitpid(pid, NULL, 0);
        lseek(fdOutput, 0, SEEK_SET);
        while (read(fdOutput, buffer, 2047) > 0) {
            newBody += buffer;
            memset(buffer, 0, 2048);
        }
    }

    dup2(saveStdin, STDIN_FILENO);
    dup2(saveStdout, STDOUT_FILENO);
    close(saveStdin);
    close(saveStdout);
    close(fdInput);
    close(fdOutput);
    fclose(fileInput);
    fclose(fileOutput);
    response = newBody;
    return ("200");
}

std::string CgiHandler::_handlePythonScript(std::string &response) {
    pid_t pid;
    std::string newBody;
    int saveStdin = dup(STDIN_FILENO);
    int saveStdout = dup(STDOUT_FILENO);

    // cria um arquivo temporario para o input e outro para o output
    FILE *fileInput = tmpfile();
    FILE *fileOutput = tmpfile();

    // pega o file descriptor do arquivo temporario
    long fdInput = fileno(fileInput);
    long fdOutput = fileno(fileOutput);

    // escreve o body da request no arquivo temporario
    write(fdInput, _request.getBody().c_str(), _request.getBody().size());
    // volta o ponteiro para o inicio do arquivo
    lseek(fdInput, 0, SEEK_SET);

    // cria um processo filho
    pid = fork();
    if (pid == -1) {
        std::cout << RED << "Error: unable to fork" << RESET << std::endl;
        return ("");
    } else if (!pid) {
        // redireciona o input e o output para os arquivos temporarios
        dup2(fdInput, STDIN_FILENO);
        dup2(fdOutput, STDOUT_FILENO);

        char *args[] = {
                const_cast<char *>(_typeHelper.getCgi().begin()->second.c_str()),
                const_cast<char *>(_typeHelper.getCgiFolder().c_str()),
                NULL
        };

        // executa o script python
        execve("/usr/bin/python3", args, _envArray);
        std::cout << RED << "Error: unable to execve" << RESET << std::endl;
        write(STDOUT_FILENO, "Content-Type: text/html\r\n\r Status: 500 Internal Server Error\r\n\r\n", 64);
    } else {
        // espera 5 segundos para o processo filho terminar
        clock_t start = clock();
        int status;
        while (true) {
            double total = (double) (clock() - start) / CLOCKS_PER_SEC;
            if (total > SECONDS) {
                // Verifica se o processo filho entrou em loop infinito
                if (waitpid(pid, &status, WNOHANG) == 0) {
                    _closeAll(fdInput, fdOutput, saveStdin, saveStdout, fileInput, fileOutput);
                    kill(pid, SIGSEGV);
                    return ("508");
                }
                kill(pid, SIGSEGV);
                break;
            }
            // verifica se o processo filho terminou antes dos 5 segundos
            if (waitpid(pid, &status, WNOHANG) == pid) {
                break;
            }
        }
        // Verifica se o processo filho retornou um erro
        if (WEXITSTATUS(status) != 0) {
            _closeAll(fdInput, fdOutput, saveStdin, saveStdout, fileInput, fileOutput);
            kill(pid, SIGSEGV);
            return ("500");
        }

        // le o arquivo temporario e coloca no body da response
        char buffer[2048] = {0};
        // espera o processo filho terminar
        waitpid(pid, NULL, 0);
        lseek(fdOutput, 0, SEEK_SET);

        // le o arquivo temporario e coloca no body da response
        while (read(fdOutput, buffer, 2047) > 0) {
            newBody += buffer;
            memset(buffer, 0, 2048);
        }
    }

    // restaura o input e o output para o terminal
    dup2(saveStdin, STDIN_FILENO);
    dup2(saveStdout, STDOUT_FILENO);
    close(saveStdin);
    close(saveStdout);
    close(fdInput);
    close(fdOutput);
    // fecha os arquivos temporarios
    fclose(fileInput);
    fclose(fileOutput);
    response = newBody;
    return ("200");
}

std::string CgiHandler::toString(std::string::size_type i) {
    std::stringstream ss;
    ss << i;
    return ss.str();
}

std::string _getContentTypeHeader(const std::string &extension) {
    std::map<std::string, std::string> extensionMap;
    extensionMap[".html"] = "text/html";
    extensionMap[".css"] = "text/css";
    extensionMap[".js"] = "application/javascript";
    extensionMap[".json"] = "application/json";
    extensionMap[".jpg"] = "image/jpeg";
    extensionMap[".jpeg"] = "image/jpeg";
    extensionMap[".png"] = "image/png";
    extensionMap[".gif"] = "image/gif";
    extensionMap[""] = "application/x-www-form-urlencoded";
    std::string contentType = extensionMap[extension];
    if (contentType.empty()) {
        contentType = "application/octet-stream";
    }
    return (contentType);
}

void CgiHandler::_initEnvironmentVariables() {
    _env["CONTENT_LENGTH"] = toString(_request.getBody().length());
    _env["CONTENT_TYPE"] = _getContentTypeHeader(_request.getExtension());
    _env["SCRIPT_NAME"] = _request.getTarget();
    _env["SCRIPT_FILENAME"] = _request.getTarget();
    _env["PATH_INFO"] = _typeHelper.getPath();
    _env["PATH_TRANSLATED"] = _typeHelper.getCgiFolder();
    _env["REMOTEaddr"] = _typeHelper.getPort();
    _env["QUERY_STRING"] = _request.getQuery();
    _env["REQUEST_URI"] = _request.getTarget() + _request.getQuery();
    _env["REQUEST_METHOD"] = _request.getMethod();
    _env["SERVER_PORT"] = _typeHelper.getPort();
    _env["SERVER_PROTOCOL"] = _request.getProtocol();
    _env["GATEWAY_INTERFACE"] = "CGI/1.1";
    _env["REDIRECT_STATUS"] = "200";
    _env["SERVER_NAME"] = "ft_webserver | 1.O";
    _env["SERVER_SOFTWARE"] = "ft_webserver | 1.O";
    _env["REMOTE_IDENT"] = "";
    _env["REMOTE_USER"] = "";
    _env["content"] = _request.getBody();
}

char **CgiHandler::_getEnvironmentVariables() {
    char **env = new char *[_env.size() + 1];
    int i = 0;
    for (std::map<std::string, std::string>::iterator it = _env.begin(); it != _env.end(); ++it) {
        std::string tmp = it->first + "=" + it->second;
        env[i] = new char[tmp.size() + 1];
        strcpy(env[i], tmp.c_str());
        i++;
    }
    env[i] = NULL;
    return env;
}

void CgiHandler::printMessage() {
    std::cout << GREEN << "Calling CGI" << RESET << std::endl;
}

