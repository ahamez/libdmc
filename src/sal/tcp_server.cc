// $Author: ahamez $
// $Revision: 969 $
// $Date: 2007-06-13 11:56:35 +0200 (mer, 13 jun 2007) $

#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <pthread.h>

#include "tcp_server.hh"

namespace dmc { namespace sal {

//////////////////////////////////////////////////////////////////////

tcp_server::tcp_server(	unsigned short port ,
						bool no_accept_thread,
						bool no_listen_thread )
    :
	fd_mutex(),
	no_accept_thread_(no_accept_thread),
	no_listen_thread_(no_listen_thread),
	is_started_(false),
	wait_sock_(),
	opened_sockets_(),
	port_(port),
	scom_(),
	wait_for_connections_tid_()
{
	pthread_mutex_init(&fd_mutex,nullptr);
}

//////////////////////////////////////////////////////////////////////

void
tcp_server::start_server()
{
	if ( !is_started_ )
	{
		if( !no_accept_thread_ )
		{
			// // create a thread tid_
			// pthread_t tid_;

			// set options
			pthread_attr_t thread_type_;
			pthread_attr_init(&thread_type_);
			pthread_attr_setschedpolicy(&thread_type_,SCHED_OTHER);

			// create and start on wait_for_connections ()
			if( pthread_create(	&wait_for_connections_tid_,
								&thread_type_,
								wait_for_connections,
								this))
			{
				perror("[tcp_server pthread_create]");
				exit(1);
			}
			else
			{
				is_started_ = true;
			}
		}
		else
		{
			wait_for_connections(this);
		}
	}
}

//////////////////////////////////////////////////////////////////////

void
tcp_server::stop_server()
{
	if ( is_started_)
	{
		shutdown(wait_sock_,SHUT_RDWR);
		opened_sockets::iterator it;
		for( it = opened_sockets_.begin() ; it != opened_sockets_.end() ; ++it )
		{
			shutdown(*it,SHUT_RDWR);
		}
		is_started_ = false;

		if( !no_accept_thread_ )
		{
			pthread_join(wait_for_connections_tid_,nullptr);
		}
	}
}

//////////////////////////////////////////////////////////////////////

// static
void*
tcp_server::wait_for_connections(void* pthis)
{

	tcp_server* _this = static_cast<tcp_server *> (pthis);

	_this->wait_sock_ = get_server_socket(_this->port_);

	while(true)
	{
    		/// enter critical region to protect fd until the new thread has copied it
		pthread_mutex_lock(&(_this->fd_mutex));


		// temporary variable to hold fd
		int scom;
		{
			struct sockaddr_in exp;
			socklen_t fromlen = sizeof(exp);

			struct timespec time;
			time.tv_sec= 0;
			time.tv_nsec = 50000;

			int tries = 0;
			while( (scom = accept(_this->wait_sock_,(struct sockaddr*)&exp,&fromlen)) == -1)
			{
				if( errno == ECONNABORTED || errno == EBADF)
				{
					break;
				}

				if( tries++ > 10 )
				{
					perror("[accept]");
					exit(1);
				}
				else
				{
					// wait 50us
					nanosleep(&time,nullptr);
				}
			}
		} // scom now is an open fd for comm with client

		// check if whe have exited from the previous loop
		// with an expected error
		if( errno == ECONNABORTED || errno == EBADF )
		{
			break;
		}

		// create a new thread for dialogue with client
		// create a thread tid_
		pthread_t tid_;

		// set options
		pthread_attr_t thread_type_;
		pthread_attr_init(&thread_type_);
		pthread_attr_setschedpolicy(&thread_type_,SCHED_OTHER);

//		/// enter critical region to protect fd until the new thread has copied it
//		pthread_mutex_lock(&(_this->fd_mutex));
		_this->scom_ = scom;

		// create and start on wait_for_connections ()
		if( ! _this->no_listen_thread_ )
		{
			if( pthread_create(&tid_ ,
							   &thread_type_,
							   &accept_connections,
							   _this ) )
			{
				perror("[tcp_server pthread_create]");
				exit(1);
			}
		}
		else
		{
			_this->pre_handle(_this);
			return nullptr;
		}

	}

	pthread_exit(nullptr);

	// avoid warning
	return nullptr;
}

//////////////////////////////////////////////////////////////////////

// static
void*
tcp_server::accept_connections(void* pthis)
{
	tcp_server* _pthis = static_cast<tcp_server*>(pthis);
	(_pthis)->pre_handle(pthis);

	// avoid warning
	return nullptr;
}

//////////////////////////////////////////////////////////////////////

void
tcp_server::pre_handle(void* pthis)
{
	tcp_server* _pthis = static_cast<tcp_server*>(pthis);
	int tmp_sock = _pthis->scom_;
	opened_sockets_.push_back(_pthis->scom_);
    pthread_mutex_unlock(&(_pthis->fd_mutex));

	// call the method of the derived class
	_pthis->handle_connection(tmp_sock);
}

//////////////////////////////////////////////////////////////////////

// static
int
tcp_server::get_server_socket(unsigned short port)
{
	struct sockaddr_in sin;
	int sc;

	// socket creation
	if (( sc = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("[tcp_server socket]");
		exit(1);
	}

	// for fast restart on same port
	int reuse = 1;
	if ( setsockopt(sc, SOL_SOCKET, SO_REUSEADDR, (int*)&reuse, sizeof(reuse)) == -1 )
	{
		perror("[tcp_server setsockopt]");
	}

	bzero((char *)&sin,sizeof(sin));
	sin.sin_addr.s_addr = htons(INADDR_ANY);
	sin.sin_port = htons(port);;
	sin.sin_family = AF_INET;

	// naming
	if(bind(sc,(struct sockaddr*) &sin, sizeof(sin)) < 0)
	{
		perror("[tcp_server bind]");
		exit(1);
	}

	// start listening on socket
	listen(sc, 32);

	return sc;
}

//////////////////////////////////////////////////////////////////////

int
tcp_server::send_data(void* data,size_t size , int fd)
{
	/// \todo handle more errors

	size_t nb_sent = 0;
	while( nb_sent < size )
	{
		int ret = ::send( fd,
						  ((char*)data) + nb_sent,
						  size - nb_sent,
						  0);

		if( ret == -1 )
		{
			perror("[::send]");
			exit(1);
		}

		nb_sent += ret;

	}

	return nb_sent;
}

//////////////////////////////////////////////////////////////////////

int
tcp_server::recv_data( void* data, size_t size , int fd)
{
	/// \todo handle more errors

	size_t nb_read = 0;
	while( nb_read < size )
	{
		int ret = ::recv( fd,
						  ((char*)data) + nb_read,
						  size - nb_read,
						  0);

		if( ret == -1 )
		{
			perror("[tcp_server_a::recv]");
			exit(1);
		}

		nb_read += ret;

		if( nb_read == 0 )
		{
			break;
		}
	}

	return nb_read;
}

//////////////////////////////////////////////////////////////////////

}} // namespaces
