#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdexcept>
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
		const bool		&isListener(void) const;
		class Server	*getServer(void) const;

		// -Setters
		void	setTypeListener(bool isListener);
		void	setServer(Server *server);

		// -Methods
		bool	bind(const std::string& address, uint16_t port);
		bool	listen(int backlog = 10);
		Socket	accept(void);
		void	connect(const std::string& address, uint16_t port);
		ssize_t	send(const void* buf, size_t len, int flags = 0);
		ssize_t	recv(char* buf, size_t len, int flags = 0);
		void	setsockopt(int level, int optname, const void* optval, socklen_t optlen);
		void	close(void);

	private:
		int				_fd;
		bool			_isListener;
		class Server*	_server;
};

// -Functions
std::ostream &operator<<(std::ostream &out, Socket const &in);

#endif
