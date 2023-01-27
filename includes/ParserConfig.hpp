#ifndef PARSERCONFIG_HPP
#define PARSERCONFIG_HPP

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
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
		const std::string	&get_file(void) const;

		// -Setters
		void	set_file(std::string _file);

		// -Methods
		void	parseFile(const std::string &FilePath, WebServer& WebServer);
	private:
		std::string		_file;
	protected:

};

// -Functions
std::ostream &operator<<(std::ostream &out, ParserConfig const &in);

#endif