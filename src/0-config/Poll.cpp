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

Poll::~Poll(void)
{
}

bool Poll::checkEvent(short event) {
	if ((event & POLLIN) == POLLIN)
		return true;
	if ((event & POLLOUT) == POLLOUT)
		return true;
	return false;
}

void	Poll::run(void)
{
	int ret = poll(this->_poolfd_list.data(), this->_size, -1);
	if (ret == -1)
		throw std::runtime_error("poll error");
}

const size_t &Poll::getSize(void) const
{
	return (this->_size);
}

Socket *Poll::getSocket(size_t index)
{
	return (this->_sockets[index]);
}

short Poll::getEventReturn(size_t index)
{
	return (this->_poolfd_list[index].revents);
}
