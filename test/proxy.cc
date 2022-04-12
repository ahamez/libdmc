#include "util/tcp_server.hh"
#include "util/tcp_client.hh"

#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

std::ofstream f_size_("dump_size");
std::ofstream f_data_("dump_data");

// For reading the data stream between two nodes

int main()
{

	/////////////////////////////////////////////////////////////////
	// starts server
	
	int server_port = 1664;
	int accept_socket = 0;
	struct sockaddr_in sin;
	
	// socket creation
	if (( accept_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("[tcp_server socket]");
		exit(1);
	}
	
	// for fast restart on same port
	int reuse = 1;
	if ( setsockopt(accept_socket, SOL_SOCKET, SO_REUSEADDR, (int*)&reuse, sizeof(reuse)) == -1 ) 
	{
		perror("[tcp_server setsockopt]");
	}
	
	bzero((char *)&sin,sizeof(sin));
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_port = htons(server_port);
	sin.sin_family = AF_INET;
	
	// naming
	if( bind(accept_socket,(struct sockaddr*) &sin, sizeof(sin)) == -1 )
	{
		perror("[tcp_server bind]");
		exit(1);
	}
	
	// start listening on socket
	listen(accept_socket, 1);
	
	struct sockaddr_in exp;
	socklen_t fromlen = sizeof(exp);
	int scom = 0;
	
	if ((scom = accept(accept_socket,(struct sockaddr*)&exp,&fromlen)) == -1)
	{
		perror("[accept]");
		exit(1);
	}
	
	
	
	/////////////////////////////////////////////////////////////////
	// starts client
	
	int client_port = 2000;
	std::string addr("127.0.0.1");
	struct sockaddr_in server;
	struct hostent *host;
	int server_size = sizeof(server);
	int sock = 0;
	
	if( (sock = socket(AF_INET, SOCK_STREAM,0)) == -1)
	{
		perror("[socket]");
		exit(1);
	}
	
	if( ( host = gethostbyname(addr.c_str()) ) == NULL)
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
	server.sin_port = htons(client_port);
	
	if(connect( sock, (struct sockaddr*)&server, server_size)  == -1 )
	{
		perror("[connect]");
		exit(1);
	}
	
	
	/////////////////////////////////////////////////////////////////	
	// listen

	  unsigned long cpt = 0;
	  unsigned int c = 0;

	char*  data = new char[1000];

	while( true )
	{
		size_t size;
		
		//
		/////////////////////////////////
		// server
		
		int nb;
		if( (nb = ::recv(scom, (char*)&(size), sizeof(size_t), 0)) == -1 )
		{
			perror("[server read 1]");
			exit(1);
		}
		
		if( nb == 0 )
		{
			std::cout << "Connexion closed." << std::endl;
			std::cout << cpt  << " States received" << std::endl;
			delete[] data;
			break;
		}
		
		f_size_ << size << std::endl;
		

		
		if( ::recv(scom,data,size,0) == -1 )
		{      
			perror("[server read 2]");
			exit(1);
		}
		
		for( size_t i = 0 ; i < size ; ++ i )
		{
			f_data_ << data[i];
		}
		f_data_ << std::endl;
		
		//
		/////////////////////////////////
		// client
		
		if( ::send(sock,&size,sizeof(size_t),0) == -1 )
		{
			perror("[client send 1]");
			exit(1);
		}

		if( ::send(sock,data,size,0) == -1 )
		{
			perror("[client send 2]");
			exit(1);
		}		
		
		//
		/////////////////////////////////
		//
		
		cpt++;
		++c;
		
		if( c == 10000 )
		  {
		    c = 0;
		    std::cout << cpt  << " States received" << std::endl;
		  }
		
	}

	return EXIT_SUCCESS;
}
