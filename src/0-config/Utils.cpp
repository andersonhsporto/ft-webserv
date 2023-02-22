#include "Utils.hpp"
#include <fstream>
#include <algorithm>
#include <set>


namespace utils {
	std::vector<std::string> splitStringBy(const std::string &stringToSplit, const char &delimiter) {
		std::string					line;
		std::vector<std::string>	split;
		std::istringstream			iss(stringToSplit);

		while (std::getline(iss, line, delimiter)) {
			split.push_back(line);
		}
		return (split);
	}

	std::vector<std::string> splitStringBy(const std::string &stringToSplit, const std::string &delimiter) {
		std::vector<std::string> split;
		std::string::size_type pos = 0;
		std::string::size_type prev = 0;
		while ((pos = stringToSplit.find(delimiter, prev)) != std::string::npos)
		{
			split.push_back(stringToSplit.substr(prev, pos - prev));
			prev = pos + delimiter.length();
		}
		split.push_back(stringToSplit.substr(prev));
		return split;
	}

	int stringToInt(const std::string &word) {
		int	wordInt;

		std::istringstream iss(word);
		iss >> wordInt;
		return (wordInt);
	}

	std::string intToString(const int num) {
		std::ostringstream ss;

		ss << num;
		return ss.str();
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

	std::string fileToString(const std::string& filename) {
		std::ifstream input(filename);
		std::stringstream buffer;

		buffer << input.rdbuf();
		return buffer.str();
	}

	void removeSubstring(std::string& str, const std::string& substr) {
		size_t pos = 0;

		while ((pos = str.find(substr, pos)) != std::string::npos) {
			str.erase(pos, substr.length());
		}
	}

	bool isComment(const std::string& line, const std::set<std::string>& commentPrefixes, int& multi) {
		std::set<std::string>::const_iterator it;

		for (it = commentPrefixes.begin(); it != commentPrefixes.end(); ++it) {
			if (line.find(*it) == 0) {
				if (*it == "/*")
					multi = 1;
				else if (*it == "*/")
					multi = 0;
				return true;
			}
		}
		if (multi == 1)
			return true;
		return false;
	}

	std::string removeComments(const std::string& str) {
		std::set<std::string>	commentPrefixes {"//", "/*", "*/", "#", "<!--"};
		std::istringstream		input(str);
		std::string				result;
		std::string				line;
		int						multi = 0;

		while (std::getline(input, line)) {
			if (!isComment(line, commentPrefixes, multi)) {
				if (result.length() > 0) {
					result += "\n";
				}
				result += line.substr(0, line.find_first_of(*(commentPrefixes.begin()), 0));
			}
		}
		return result;
	}
}
