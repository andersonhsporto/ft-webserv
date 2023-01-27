#ifndef SERVERLOCATION_HPP
#define SERVERLOCATION_HPP

#include <iostream>
#include <vector>

class ServerLocation {
 public:
  // -Constructors
  ServerLocation(void);
  ServerLocation(ServerLocation const &rhs);

  // -Destructor
  ~ServerLocation(void);

  // -Operators
  ServerLocation &operator=(ServerLocation const &rhs);

  // -Getters

  // -Setters

  // -Methods

 private:
  // /location
  std::string path;

  // root /var/www/localhost/htdocs;
  std::string root;

  // limit_except GET POST;
  std::vector<std::string> resquestsHttp;

 protected:

};

// -Functions
std::ostream &operator<<(std::ostream &out, ServerLocation const &in);

#endif
