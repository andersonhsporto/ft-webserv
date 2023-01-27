#ifndef PARSERCONFIG_HPP
#define PARSERCONFIG_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include "WebServer.hpp"

class ParserConfig {
 public:
  // -Constructors
  ParserConfig(void);
  ParserConfig(ParserConfig const &rhs);

  // -Destructor
  ~ParserConfig(void);

  // -Operators
  ParserConfig &operator=(ParserConfig const &rhs);

  // -Getters
  const std::string	&getFile(void) const;

  // -Setters
  void	setFile(std::string _file);

  // -Methods
  // correct one void	parseFile(const std::string &FilePath, WebServer& WebServer);
  void	parseFile(const std::string &FilePath);
 private:
  std::string					_file;
  std::vector<std::string>	_configServers;

  inline void		_openFile(const std::string &FilePath);
  inline void		_splitServers();
  inline bool       _isCurlyBracketBalanced(std::string fileContent);
 protected:

};

// -Functions
std::ostream &operator<<(std::ostream &out, ParserConfig const &in);

#endif