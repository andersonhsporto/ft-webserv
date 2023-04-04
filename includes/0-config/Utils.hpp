#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>
#include <sys/stat.h>

namespace utils {
	std::vector<std::string>	splitStringBy(const std::string &stringToSplit, const char &delimiter);
	std::vector<std::string>	splitStringBy(const std::string &stringToSplit, const std::string &delimiter);
	int							stringToInt(const std::string &word);
	std::string					intToString(const int num);
	void						divideByDelimiter(const std::string &total, std::string &word1, std::string &word2, char delimiter);
	void 						trimChar(std::string &str, char toTrim);
	int							fileToString(const std::string& filename, std::string& fileContents);
	void						removeSubstring(std::string& str, const std::string& substr);
	std::string					removeComments(const std::string& str);
	std::string					getCurrentTime(void);
	bool						fileExist(std::string path);
	bool						insertStringIntoFile(std::string file_path, std::string str);
	std::string					pathIs(std::string path);
	void						addServerNametoList(std::vector<std::string> names, std::string port);
}

#endif
