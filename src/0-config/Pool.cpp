#include "./../../includes/0-config/Pool.hpp"

Poll::Poll(void)
{
	return ;
}

void Poll::init(std::vector<Socket *> &sockets)
{
	this->_size = sockets.size();
	this->_sockets = sockets;
	this->_poolfd_list = new struct pollfd[_size];

	for (size_t i = 0; i < this->_size; i++)
	{
		this->_poolfd_list[i].fd = sockets[i]->getFd();
		this->_poolfd_list[i].events = POLLIN | POLLPRI | POLLOUT | POLLWRBAND;
	}
}

Poll::~Poll(void)
{
	delete[] this->_poolfd_list;
}

void	Poll::run(void)
{
	int ret = poll(this->_poolfd_list, this->_size, 0);
	if (ret == -1)
		throw ("poll error\n");

}

Socket	* Poll::get_socket(size_t index)
{
	return (this->_sockets[index]);
}

short	Poll::get_event_return(size_t index)
{
	return (this->_poolfd_list[index].revents);
}
