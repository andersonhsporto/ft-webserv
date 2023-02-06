#ifndef PARSERUTILS_HPP
#define PARSERUTILS_HPP

#include <sstream>
#include <string>
#include <vector>

namespace parser {
	std::vector<std::string>	splitStringBy(const std::string &stringToSplit, const char &delimiter);
	std::vector<std::string>	splitStringBy(const std::string &stringToSplit, const std::string &delimiter);
	int							stringToInt(const std::string &word);
	void						divideByDelimiter(const std::string &total, std::string &word1, std::string &word2, char delimiter);
	void 						trimChar(std::string &str, char toTrim);
};

#endif