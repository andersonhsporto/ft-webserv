#include "ParserUtils.hpp"

namespace parser {
	std::vector<std::string> splitStringBy(const std::string &stringToSplit, char delimiter) {
		std::string					line;
		std::vector<std::string>	split;
		std::istringstream			iss(stringToSplit);

		while (std::getline(iss, line, delimiter)) {
			split.push_back(line);
		}
		return (split);
	}

	int stringToInt(const std::string &word) {
		int	wordInt;

		std::istringstream iss(word);
		iss >> wordInt;
		return (wordInt);
	}

	void divideByDelimiter(const std::string &total, std::string &word1, std::string &word2, char delimiter) {
		std::string::size_type	pos;

		pos = total.find(delimiter);
		word1 = total.substr(0, pos);
		word2 = total.substr(pos + 1);
		return ;
	}

	void trimChar(std::string &str, char toTrim) {
		std::size_t first;
		std::size_t last;

		first = str.find_first_not_of(toTrim);
		last = str.find_last_not_of(toTrim) + 1;
		if (first != std::string::npos && last != std::string::npos && first < last) {
			str.substr(first, last - first);
		}
		return ;
	}
}
