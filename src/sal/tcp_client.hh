// $Author: ahamez $
// $Revision: 1026 $
// $Date: 2008-03-29 13:06:46 +0100 (sam, 29 mar 2008) $

#ifndef DMC_TCP_CLIENT_HH_
#define DMC_TCP_CLIENT_HH_

#include <string>

namespace dmc { namespace sal {

//////////////////////////////////////////////////////////////////////

class tcp_client
{

// attributes
private:

	///
	bool connected_;
	///
	int socket_;
	///
	std::string serv_name_;
	///
	unsigned short serv_port_;


// methods
public:

	tcp_client( const std::string& host,
				unsigned short port);
  	virtual ~tcp_client();

	void
	connect();

	void
	disconnect();

protected:

	int
	send_data( 	void* data,
				size_t size);

	int
	recv_data( 	void* data,
				size_t size);


private:

	int
	get_client_socket( 	const std::string& ip,
						unsigned short port);

};

//////////////////////////////////////////////////////////////////////

}} // namespaces

#endif
