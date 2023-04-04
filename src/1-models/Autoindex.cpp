#include "Autoindex.hpp"
#include <iostream>

Autoindex::Autoindex(void) {return ;}

Autoindex::~Autoindex(void) {return ;}

std::string Autoindex::autoindexPageGenerator(const std::string &target, std::string &page, const Server &serve) {
  std::string path = serve.getRoot() + target;
  DIR *dir = opendir(path.c_str());
  if (dir == NULL){
    std::cout << "Error: could not open this folder!\n";
    return "404";
  }
  page = "<!DOCTYPE html>\n\
        <html>\n\
        <head>\n\
        <title>" + path + "</title>\n\
        </head>\n\
        <body>\n\
        <h1>INDEX</h1>\n\
        <p>\n";

  for(struct dirent *dirEntry = readdir(dir); dirEntry; dirEntry = readdir(dir))
    page += _linkGenerator(target, std::string(dirEntry->d_name), serve);
  page += "\
          </p>\n\
          </body>\n\
          </html>\n";
  closedir(dir);
  return "200";
}

std::string Autoindex::_linkGenerator(const std::string &pathLink, std::string dirEntry, const Server &server){
  std::stringstream ss;
  ss << "\t\t<p><a href=\"http://localhost" << ":" << server.getPort()
     << pathLink + (pathLink[pathLink.size() - 1] != '/' ? "/" : "") + dirEntry + "\">" + dirEntry + "</a></p>\n";
  return ss.str();
}