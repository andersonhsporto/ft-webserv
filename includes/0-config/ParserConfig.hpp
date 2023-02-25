#ifndef PARSERCONFIG_HPP
#define PARSERCONFIG_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <map>
#include <arpa/inet.h>

class ParserConfig {
	private:
		// -Typedefs
		typedef void (*_parseServerFunc)(const std::string &value, class Server &server);

	public:
		// -Constructors
		ParserConfig(class WebServer& webserver);

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
		class WebServer							&_webServer;
		std::vector<std::string>				_configServers;
		std::map<std::string, _parseServerFunc>	_parseFuncs;
		inline bool			_isCurlyBracketBalanced(std::string fileContent);
		inline void			_splitServers();
		inline void			_setServers();
		static void			_parseListen(const std::string &value, class Server &server);
		static void			_parseServerName(const std::string &value, class Server &server);
		static void			_parseMaxSizeBody(const std::string &value, class Server &server);
		static void			_parseRoot(const std::string &value, class Server &server);
		static void			_parseIndex(const std::string &value, class Server &server);
		static void			_parseErrorPage(const std::string &value, class Server &server);
		static void			_parseTimeOut(const std::string &value, class Server &server);
		static void			_parseLocation(const std::string &value, class Server &server);
		static void			_parseCgi(const std::string &value, class Server &server);
};

// -Functions
std::ostream &operator<<(std::ostream &out, ParserConfig const &in);

#endif
