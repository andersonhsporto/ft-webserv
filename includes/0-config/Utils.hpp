#ifndef UTILS_HPP
#define UTILS_HPP

#include <sstream>
#include <string>
#include <vector>

namespace utils {
	std::vector<std::string>	splitStringBy(const std::string &stringToSplit, const char &delimiter);
	std::vector<std::string>	splitStringBy(const std::string &stringToSplit, const std::string &delimiter);
	int							stringToInt(const std::string &word);
	std::string					intToString(const int num);
	void						divideByDelimiter(const std::string &total, std::string &word1, std::string &word2, char delimiter);
	void 						trimChar(std::string &str, char toTrim);
	std::string					fileToString(const std::string& filename);
	void						removeSubstring(std::string& str, const std::string& substr);
};

#endif
