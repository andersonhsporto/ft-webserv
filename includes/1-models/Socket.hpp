#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdexcept>
#include <netdb.h>
#include <unistd.h>

class Socket {
	public:
		// -Constructors
		Socket();
		Socket(int fd);
		Socket(Socket const &rhs);

		// -Destructor
		~Socket();

		// -Operators
		Socket &operator=(Socket const &rhs);

		// -Getters
		const int		&getFd(void) const;
		class Server	*getServer(void) const;

		// -Setters
		void	setServer(Server *server);

		// -Methods
		bool	bind(void);
		bool	listen(int backlog = 10);
		int		accept(void);
		void	setsockopt(int level, int optname, const void* optval, socklen_t optlen);
		void	close(void);

	private:
		int				_fd;
		class Server*	_server;
};

// -Functions
std::ostream &operator<<(std::ostream &out, Socket const &in);

#endif

