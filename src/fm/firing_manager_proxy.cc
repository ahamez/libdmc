// $Author: ahamez $
// $Revision: 1022 $
// $Date: 2008-03-28 22:08:39 +0100 (ven, 28 mar 2008) $

#include "firing_manager_proxy.hh"
#include "firing_manager_protocol.hh"

namespace dmc { namespace fm {

//////////////////////////////////////////////////////////////////////

firing_manager_proxy::firing_manager_proxy( conf::configuration* conf,
											int node)
    :
	sal::tcp_client( conf->get_node_hostname(node),
					  conf->get_fm_server_port(node)),
	conf_(conf)
{
}

//////////////////////////////////////////////////////////////////////

void
firing_manager_proxy::process_initial_state()
{
    connect();

	int msg = DMC_PROCESS_INITIAL;
    send_data(&msg,sizeof(int));
}

//////////////////////////////////////////////////////////////////////

void
firing_manager_proxy::start_generation()
{
    connect();

	int msg = DMC_GENERATE;
    send_data(&msg,sizeof(int));
}

//////////////////////////////////////////////////////////////////////

void
firing_manager_proxy::stop_generation()
{
    connect();

	int msg = DMC_STOP;
    send_data(&msg,sizeof(int));
	this->disconnect();
}

//////////////////////////////////////////////////////////////////////

base::status_t
firing_manager_proxy::get_status()
{
    connect();

	int msg = DMC_STATUS;
    send_data(&msg,sizeof(int));

	base::status_t res;
	recv_data(&(res.empty_set),sizeof(bool));
	recv_data(&(res.nb_sent),sizeof(unsigned long));
	recv_data(&(res.nb_recv),sizeof(unsigned long));

	return res;
}

//////////////////////////////////////////////////////////////////////

base::stats_t*
firing_manager_proxy::get_stats()
{
    connect();

	int msg = DMC_STATS;
	send_data(&msg,sizeof(int));

	auto   res = new base::stats_t;

	recv_data(&(res->set_size),sizeof(unsigned int));
	recv_data(&(res->nb_states),sizeof(unsigned long));
	recv_data(&(res->nb_succ),sizeof(unsigned long));

	res->sent_nodes = new unsigned long[conf_->get_nb_nodes()];

	recv_data( res->sent_nodes,
			   sizeof(unsigned long) * conf_->get_nb_nodes() );

	return res;
}

//////////////////////////////////////////////////////////////////////

}} // namespaces
