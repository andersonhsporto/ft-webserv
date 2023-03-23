#ifndef AUTOINDEX_HPP
#define AUTOINDEX_HPP

#include <iostream>
#include "../1-models/Server.hpp"
#include <dirent.h>

class Autoindex
{
	private:
		std::string _linkGenerator(std::string &pathLink, std::string dirEntry, const Server &server);

	public:
		Autoindex();
		~Autoindex();
		std::string autoindexPageGenerator(std::string &path, std::string &page, const Server &server);
};

#endif