#include "Poll.hpp"
#include <iostream>

Poll::Poll(void) {
	return ;
}

void Poll::init(void) {
	int size;

	size = this->_sockets.size();
	this->_poolfd_list.resize(size);
	for (int i = 0; i < size; i++) {
		this->_poolfd_list[i].fd = this->_sockets[i]->getFd();
		this->_poolfd_list[i].events = POLLIN | POLLOUT;
	}
}

void Poll::addSocket(Socket *newSocket){
	pollfd newPollFD;

	this->_sockets.push_back(newSocket);
	newPollFD.fd = newSocket->getFd();
	newPollFD.events = POLLIN | POLLOUT;
	newPollFD.revents = 0;
	this->_poolfd_list.push_back(newPollFD);
}

void Poll::deleteSocket(Socket *socket) {
	Socket *deletedSocket;
	for(std::vector<pollfd>::reverse_iterator it = _poolfd_list.rbegin(); it != _poolfd_list.rend(); ++it){
		if(it->fd == socket->getFd()){
			_poolfd_list.erase(std::vector<pollfd>::iterator(&(*it)));
			break;
		}
	}

	for(std::vector<Socket *>::reverse_iterator it = _sockets.rbegin(); it != _sockets.rend(); ++it){
		if((*it) == socket){
			deletedSocket = *it;
			_sockets.erase(std::vector<Socket *>::iterator(&(*it)));
			delete deletedSocket;
			break;
		}
	}
}

Poll::~Poll(void) {
	for(size_t i = 0; i < this->_sockets.size(); i++)
		delete this->_sockets[i];
}

bool Poll::checkEvent(short event) {
	if ((event & POLLIN) == POLLIN)
		return true;
	if ((event & POLLOUT) == POLLOUT)
		return true;
	return false;
}

void	Poll::run(void) {
	std::cout << "Parsed FD list: " ;
	for (std::vector<pollfd>::iterator it = _poolfd_list.begin(); it != _poolfd_list.end() ; ++it) {
		std::cout << it->fd << ", ";
	}
	std::cout << "\n";
	int ret = poll(this->_poolfd_list.data(), this->getSize(), -1);
	if (ret == -1)
		throw std::runtime_error("ERROR: poll error\n");
}

size_t Poll::getSize(void) const {
	return (this->_poolfd_list.size());
}

Socket *Poll::getSocket(size_t index) {
	return (this->_sockets[index]);
}

short Poll::getEventReturn(size_t index)
{
	return (this->_poolfd_list[index].revents);
}

void Poll::clear(void){
	Socket *ptr;
	for(int index = this->_poolfd_list.size() - 1; index >= 0; --index){
		std::cout << "Closing FD " << _poolfd_list[index].fd << "\n";
		::close(_poolfd_list[index].fd);
		_poolfd_list.pop_back();
	}
	for(int index = this->_sockets.size() - 1; index >= 0; --index){
		ptr = _sockets[index];
		_sockets.pop_back();
		delete ptr;
	}
}
