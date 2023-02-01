#ifndef PARSERUTILS_HPP
#define PARSERUTILS_HPP

#include <sstream>
#include <string>
#include <vector>

namespace parser {
	std::vector<std::string>	splitStringBy(const std::string &stringToSplit, char delimiter);
	int							stringToInt(const std::string &word);
	void						divideByDelimiter(const std::string &total, std::string &word1, std::string &word2, char delimiter);
};

#endif