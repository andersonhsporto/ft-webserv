#ifndef POLL_HPP
#define POLL_HPP

#include "Socket.hpp"
#include <vector>
#include <poll.h>

class Poll {
	private:
		std::vector<Socket *>	_sockets;
		std::vector<pollfd>		_poolfd_list;

	public:
		Poll();
		~Poll();

		Socket			*getSocket(size_t index);
		short			getEventReturn(size_t index);
		void			resetEventReturn(size_t index);
		size_t			getSize(void) const;
		void 			addSocket(Socket *newSocket);
		void			deleteSocket(Socket *socket);
		bool			checkEvent(short event);
		void			run();
		void			clear(void);
};

#endif