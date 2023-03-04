#include "Poll.hpp"
#include <iostream>

Poll::Poll(void)
{
	return ;
}

void Poll::init(const std::vector<Socket *> &sockets)
{
	this->_size = sockets.size();
	this->_sockets = sockets;
	this->_poolfd_list.resize(this->_size);

	for (size_t i = 0; i < this->_size; i++)
	{
		this->_poolfd_list[i].fd = sockets[i]->getFd();
		this->_poolfd_list[i].events = POLLIN | POLLOUT;
	}
}

void Poll::addSocket(Socket *newSocket){
	pollfd newPollFD;

	this->_sockets.push_back(newSocket);
	newPollFD.fd = newSocket->getFd();
	newPollFD.events = POLLIN | POLLOUT;
	this->_poolfd_list.push_back(newPollFD);
}

Poll::~Poll(void)
{
}

bool Poll::checkEvent(short event) {
	if ((event & POLLIN) == POLLIN)
		return true;
	if ((event & POLLOUT) == POLLOUT)
		return true;
	std::cout << "valor de retorno: " << event << "\n";
	return false;
}

void	Poll::run(void)
{
	std::cout << "poll::run) lista de fd analisadas: " ;
	for (std::vector<pollfd>::iterator it = _poolfd_list.begin(); it != _poolfd_list.end() ; ++it) {
		std::cout << it->fd << ", ";
	}
	std::cout << "\n";
	int ret = poll(this->_poolfd_list.data(), this->_size, -1);
	if (ret == -1)
		throw std::runtime_error("poll error");
}

const size_t Poll::getSize(void) const
{
	return (this->_poolfd_list.size());
}

Socket *Poll::getSocket(size_t index)
{
	return (this->_sockets[index]);
}

short Poll::getEventReturn(size_t index)
{
	return (this->_poolfd_list[index].revents);
}
