#ifndef POLL_HPP
#define POLL_HPP

#include "Socket.hpp"
#include <vector>
#include <poll.h>

class Poll
{
	private:
		size_t					_size;
		std::vector<Socket *>	_sockets;
		struct pollfd			*_poolfd_list;


	public:
		Poll(void);
		~Poll();
	
		void		init(std::vector<Socket *> &sockets);
		void		run(void);
		Socket		*get_socket(size_t index);
		short		get_event_return(size_t index);
};
#endif
