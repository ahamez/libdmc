// $Author: ahamez $
// $Revision: 972 $
// $Date: 2007-06-14 23:26:43 +0200 (jeu, 14 jun 2007) $

#include <sys/socket.h>

#include "state_manager_proxy.hh"

namespace dmc { namespace sm {

//////////////////////////////////////////////////////////////////////

state_manager_proxy::state_manager_proxy( conf::configuration* conf,
                                          int node_to_connect,
										  stats::stats& stats,
                                          pthread_mutex_t* mut)
    :
	sal::tcp_client( conf->get_node_hostname(node_to_connect),
                      conf->get_sm_server_port(node_to_connect)),
	stats_(stats),
	mut_(mut)
{
}

//////////////////////////////////////////////////////////////////////

void
state_manager_proxy::process_state(rg::dmc_state* as)
{
	pthread_mutex_lock(mut_);

	connect();

	send_data( &(as->size), sizeof(size_t) );
	send_data( as->data, as->size );
	send_data( &(as->distance_to_initial_), sizeof(unsigned long) );

	pthread_mutex_unlock(mut_);

	delete as;

	stats_.notify_state_sent();
}

//////////////////////////////////////////////////////////////////////

}} // namespaces
