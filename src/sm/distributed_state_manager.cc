// $Author: lbautista $
// $Revision: 1036 $
// $Date: 2008-04-14 17:10:01 +0200 (Mon, 14 Apr 2008) $

#include "distributed_state_manager.hh"
#include "mon/activity_monitor_proxy.hh"
#include "sm/state_manager_proxy.hh"
#include "state_manager.hh"

namespace dmc { namespace sm {

//////////////////////////////////////////////////////////////////////

distributed_state_manager::distributed_state_manager( conf::configuration* conf,
													  stats::stats& stats,
													  rg::new_states_set* nss)
	:
	conf_(conf),
	stats_(stats),
	tab_sm_( conf_->get_nb_nodes(),
			 (abstract_state_manager*)nullptr),
	sm_server_()
{
	tab_sm_[conf_->get_node_id()] = new sm::state_manager( conf_, nss);

	sm_server_ = new sm::state_manager_service( conf_,
												 stats_,
												 tab_sm_[conf_->get_node_id()]); // the concrete sm

	// start to listen to incoming states from the other nodes
	// create of a new thread
	sm_server_->start_server();
	pthread_mutex_init(&proxies_creation_mutex_,nullptr);
	pthread_mutex_init(&send_mut_,nullptr);
}

//////////////////////////////////////////////////////////////////////

distributed_state_manager::~distributed_state_manager()
{
	sm_server_->stop_server();
	delete sm_server_;

	for(auto & elem : tab_sm_)
	{
		// because some entries may have not been initialized
		if( elem != nullptr )
		{
			delete elem;
		}
	}

	pthread_mutex_destroy(&proxies_creation_mutex_);
	pthread_mutex_destroy(&send_mut_);
}

//////////////////////////////////////////////////////////////////////

void
distributed_state_manager::process_state(rg::dmc_state* as)
{
	// ask the state which node it belongs to
    int n = as->localization() % conf_->get_nb_nodes();

    pthread_mutex_lock(&proxies_creation_mutex_);

	stats_.notify_state_sent_to_node(n);

	// check if a state_manager, proxy or not, already exists for this node
	if ( !tab_sm_[n] )
	{
		// create a new state_manager proxy for a distant node
		tab_sm_[n] = new sm::state_manager_proxy( conf_,
												   n,
												   stats_,
												   &send_mut_);
	}

    pthread_mutex_unlock(&proxies_creation_mutex_);

	// finally, process the state, which means:
	//	- local : check if the state is new
	//	- distant : send the state to its owner
    tab_sm_[n]->process_state(as);
}

//////////////////////////////////////////////////////////////////////

}} // namespaces
