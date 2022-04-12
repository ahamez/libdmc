// $Author: ahamez $
// $Revision: 972 $
// $Date: 2007-06-14 23:26:43 +0200 (jeu, 14 jun 2007) $

#include "state_manager_service.hh"

namespace dmc { namespace sm {

//////////////////////////////////////////////////////////////////////

state_manager_service::state_manager_service( 	conf::configuration* conf,
												stats::stats& stats,
												sm::abstract_state_manager* concrete)
	:
	sal::tcp_server(conf->get_my_sm_server_port()),
	stats_(stats),
	concrete_(concrete)
{
}

//////////////////////////////////////////////////////////////////////

void
state_manager_service::handle_connection(int fd)
{
	rg::dmc_state* s;
	bool breaked = false;

	while(true)
	{
		s = new rg::dmc_state();

		if( recv_data( &(s->size) , sizeof(size_t) , fd ) == 0 )
		{
			breaked = true;
			break;
		}

        s->data = new char[s->size];

		recv_data(s->data, s->size, fd);
		recv_data(&(s->distance_to_initial_),sizeof(unsigned long),fd);

		stats_.notify_state_received();
		concrete_->process_state(s);
	}

	// to ensure that we delete the last dummy created state
	if( breaked && s!= nullptr )
	{
		delete s;
	}
}

//////////////////////////////////////////////////////////////////////

}} // namespaces
