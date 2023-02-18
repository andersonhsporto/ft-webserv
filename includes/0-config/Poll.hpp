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
		std::vector<pollfd>		_poolfd_list;

	public:
		Poll();
		~Poll();

		Socket			*getSocket(size_t index);
		short			getEventReturn(size_t index);
		const size_t	&getSize(void) const;
		void			init(const std::vector<Socket *> &sockets);
		void			run();
};

#endif