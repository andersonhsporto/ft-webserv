#include "Autoindex.hpp"
#include <iostream>

Autoindex::Autoindex(void) {return ;}

Autoindex::~Autoindex(void) {return ;}

std::string Autoindex::autoindexPageGenerator(std::string &path, std::string &page, const Server &serve){
     DIR *dir = opendir(path.c_str());
     if (dir == NULL){
        std::cout << "Error: não foi possivel abrir este diretorio!\n";
        return "404";
     }
    page = "<!DOCTYPE html>\n\
    <html>\n\
    <head>\n\
    </head>\n\
    <body>\n\
    <h1>INDEX</h1>\n\
    <p>\n";

    for(struct dirent *dirEntry = readdir(dir); dirEntry; dirEntry = readdir(dir))
      page += _linkGenerator(path, std::string(dirEntry->d_name), serve);
    page += "\
    </p>\n\
    </body>\n\
    </html>\n";
    closedir(dir);
    return "200";
}

std::string Autoindex::_linkGenerator(std::string &pathLink, std::string dirEntry, const Server &server){
  std::stringstream ss;
  ss << "\t\t<p><a href=\"http://localhost" << ":" << server.getPort() << "/"
     << pathLink + "/" + dirEntry + "\">" + dirEntry + "</a></p>\n";
  return ss.str();
}