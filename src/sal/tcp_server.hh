// $Author: ahamez $
// $Revision: 972 $
// $Date: 2007-06-14 23:26:43 +0200 (jeu, 14 jun 2007) $

#ifndef DMC_TCP_SERVER_HH_
#define DMC_TCP_SERVER_HH_

#include <pthread.h>
#include <vector>

namespace dmc { namespace sal {

	typedef std::vector<int> opened_sockets;

//////////////////////////////////////////////////////////////////////

/// An abstract class for multi-threaded TCP server services.
/// Derive from this class and implement handle_connection to get instant multi-threaded behavior.
/// Inside handle_connection, fd is a connected file descriptor, and you are a new thread.
/// Implement clone() to inherit associations.
/// \todo Seperate listen() and accept() in two differents objects => tcp_server encapsulates
/// objects which overload operator()?

class tcp_server
{

// attributes
private:

	/// A mutex to protect fd variable
	pthread_mutex_t fd_mutex;
	/// Indicate that just one accepting socket is necessary
	bool no_accept_thread_;
	/// Indicate that just one listening socket is necessary
	bool no_listen_thread_;
	/// True if a waiting thread is running
	bool is_started_ ;
	/// Accept socket
	int wait_sock_;
	/// Contains all opened socket by this server
	opened_sockets opened_sockets_;
	/// Port to listen on
	unsigned short port_;
	/// File descriptor with open communication
	int scom_;
	///
	pthread_t wait_for_connections_tid_;

// methods
public:

	tcp_server( unsigned short port,
                bool no_accept_thread = false,
                bool no_listen_thread = false);
	virtual ~tcp_server(){};

	void
	start_server();

	void
	stop_server();

	virtual void
	handle_connection (int fd) = 0;

protected:

    int
	send_data( void* data, size_t size, int fd);

	int
	recv_data( void* data, size_t size, int fd);

private:

    /// Used to be able to store open sockets
	void pre_handle(void*);

	/// private method used as entry point of a new thread when start is called.
	/// Read argument as of type this :   tcp_server *
	static void* wait_for_connections(void*);

	/// private method used as entry point of a new thread when accept is successful.
	static void* accept_connections (void*);
	static int get_server_socket(unsigned short);

};

//////////////////////////////////////////////////////////////////////

}} // namespaces

#endif
