#ifndef PARSERCONFIG_HPP
#define PARSERCONFIG_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include "WebServer.hpp"

class ParserConfig {
	private:
		// -Typedefs
		typedef void (*_parseServerFunc)(const std::string &value);

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
		void	parseFile(const std::string &FilePath);
	private:
		std::string								_file;
		std::vector<std::string>				_configServers;
		WebServer								_webServer;
		std::map<std::string, _parseServerFunc>	_parseFuncs;

		inline void		_openFile(const std::string &FilePath);
		inline bool		_isCurlyBracketBalanced(std::string fileContent);
		inline void		_splitServers();
		inline void		_setServers();
		static void		_parseListen(const std::string &value);
		static void		_parseServerName(const std::string &value);
		static void		_parseMaxSizeBody(const std::string &value);
		static void		_parseRoot(const std::string &value);
		static void		_parseIndex(const std::string &value);
		static void		_parseErrorPage(const std::string &value);
		static void		_parseLocation(const std::string &value);
	protected:
};

// -Functions
std::ostream &operator<<(std::ostream &out, ParserConfig const &in);

#endif