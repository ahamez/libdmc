// $Author: ahamez $
// $Revision: 1023 $
// $Date: 2008-03-29 00:52:26 +0100 (sam, 29 mar 2008) $

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <iostream>
#include <cstdlib>
#include <cstring>

#include "tcp_client.hh"

namespace dmc { namespace sal {

//////////////////////////////////////////////////////////////////////

tcp_client::tcp_client(	const std::string& host,
						unsigned short port)
	:
	connected_(false),
	socket_(),
	serv_name_(host),
	serv_port_(port)
{
	//connect();
}

//////////////////////////////////////////////////////////////////////

tcp_client::~tcp_client()
{
	disconnect();
}

//////////////////////////////////////////////////////////////////////

void
tcp_client::connect()
{
	if( !connected_ )
	{
		socket_ = get_client_socket( 	serv_name_,
					     				serv_port_);
		connected_ = true;
 	}
}

//////////////////////////////////////////////////////////////////////

void
tcp_client::disconnect()
{
	if( connected_ )
	{
		// disconnect from server
		shutdown(socket_,SHUT_RDWR);
		connected_ = false;
	}
}

//////////////////////////////////////////////////////////////////////

int
tcp_client::get_client_socket(	const std::string& ip,
								unsigned short port)
{
	std::cerr << "First connection to  " << ip.c_str() << ":"<< port << std::endl ;

	int sock = 0;
	struct sockaddr_in server;
	struct hostent *host;
	int server_size = sizeof(server);

	int tries = 0;
	while( tries++ < 10)
	{

		if( (sock = socket(AF_INET, SOCK_STREAM,0)) == -1)
		{
			perror("[socket]");
			exit(1);
		}

		const char* tmp = ip.c_str();
		if( ( host = gethostbyname(tmp) ) == NULL)
		{
			perror("[gethostbyname]");
			exit(1);
		}

		memset( (char*)&server,
				0,
				server_size);

		bcopy( host->h_addr,
			   (char*)&server.sin_addr,
			   host->h_length);

		server.sin_family = AF_INET;
		server.sin_port = htons(port);

		if( ::connect( sock, (struct sockaddr*)&server, server_size)  == -1 )
		{
			// connection failed, we wait a little before trying again
			// std::cerr << "Connection failed, waiting a little." << std::endl;
			close(sock);
			sleep(1);
		}
		else
		{
			// connection successful
			connected_ = true;
			break;
		}
	}

	if( connected_ == false )
	{
		perror("[connect]");
		exit(1);
	}

	return sock;
}

//////////////////////////////////////////////////////////////////////

int
tcp_client::send_data(void* data,size_t size)
{
	/// \todo handle more errors
	connect();

	size_t nb_sent = 0;
	while( nb_sent < size )
	{
		int ret = ::send( socket_,
						  ((char*)data) + nb_sent,
						  size - nb_sent,
						  0);

		if( ret == -1 )
		{
			perror("[tcp_client_a::send]");
			exit(1);
		}

		nb_sent += ret;

	}

	return 0;
}

//////////////////////////////////////////////////////////////////////

int
tcp_client::recv_data( void* data, size_t size)
{
	/// \todo handle more errors

	size_t nb_read = 0;
	while( nb_read < size )
	{
		int ret = ::recv( socket_,
						  ((char*)data) + nb_read,
						  size - nb_read,
						  0);

		if( ret == -1 )
		{
			perror("[tcp_client_b::recv]");
			exit(1);
		}

		nb_read += ret;

		if( nb_read == 0 )
		{
			break;
		}
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////

}} // namespaces
