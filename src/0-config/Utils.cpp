#include "Utils.hpp"
#include <set>
#include <ctime>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <iostream>

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
			str = str.substr(first, last - first);
		}
		return ;
	}

	int fileToString(const std::string& filename, std::string& fileContents) {
		std::ifstream input(filename.c_str());
		if (!input) {
			return (-1);
		}
		std::stringstream buffer;
		buffer << input.rdbuf();
		fileContents = buffer.str();
		return (1);
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
		const char* comment_prefixes[] = {"//", "/*", "*/", "#", "<!--"};
		const int comment_prefixes_size = sizeof(comment_prefixes) / sizeof(comment_prefixes[0]);
		std::set<std::string>	commentPrefixes(comment_prefixes, comment_prefixes + comment_prefixes_size);
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

	std::string getCurrentTime(void) {
		char timeStr[80];
		std::time_t now = std::time(NULL);
		std::tm* tm = std::gmtime(&now);
		std::strftime(timeStr, 80, "%a, %d %b %Y %H:%M:%S GMT", tm);
		return std::string(timeStr);
	}

	bool fileExist(std::string path){
  		struct stat buffer;
  		return (stat(path.c_str(), &buffer) == 0);
	}

	bool insertStringIntoFile(std::string file_path, std::string str){
		std::ofstream new_file;

		new_file.open(file_path.c_str(), std::ios::binary);
		if (!new_file.is_open())
			return false;
		new_file.write(str.c_str(), str.length());
		new_file.close();
		return true;
	}

	std::string pathIs(std::string path){
		struct stat s;

		if (stat(path.c_str(), &s) == 0){
			if (s.st_mode & S_IFDIR)
				return ("dir");
			else if (s.st_mode & S_IFREG)
				return ("file");
		}
		return ("error");
	}

	void addServerNametoList(std::vector<std::string> names) {
		std::string contents;
		std::string loopback = "127.0.0.1  "; 
		std::ofstream outfile("/etc/hosts", std::ios::app);

		if (!outfile) {
			std::cerr << "Error: Could not open file /etc/hosts" << std::endl;
			return;
		}
		if (utils::fileToString("/etc/hosts", contents) == -1) {
			std::cerr << "Error: could not capture contents of /etc/hosts file" << std::endl;
			return;
		}
		for (std::vector<std::string>::iterator it = names.begin(); it != names.end(); ++it) {
			if(contents.find(loopback + *it) == std::string::npos)
				outfile << loopback << *it << "\n";
		}
		outfile.close();
	}
}
