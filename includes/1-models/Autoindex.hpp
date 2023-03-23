#ifndef AUTOINDEX_HPP
#define AUTOINDEX_HPP

#include <iostream>
#include "../1-models/Server.hpp"
#include <dirent.h>

class Autoindex
{
	private:
		std::string _linkGenerator(const std::string &pathLink, std::string dirEntry, const Server &server);

	public:
		Autoindex();
		~Autoindex();
		std::string autoindexPageGenerator(const std::string &target, std::string &page, const Server &server);
};

#endif