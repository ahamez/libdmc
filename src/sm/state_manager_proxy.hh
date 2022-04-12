// $Author: lbautista $
// $Revision: 1029 $
// $Date: 2008-04-03 16:15:41 +0200 (jeu, 03 avr 2008) $

#ifndef DMC_SM_PROXY_HH_
#define DMC_SM_PROXY_HH_

#include "sm/abstract_state_manager.hh"
#include "conf/configuration.hh"
#include "sal/tcp_client.hh"
#include "stats/stats.hh"

namespace dmc { namespace sm {

//////////////////////////////////////////////////////////////////////

class state_manager_proxy
	:
	public sm::abstract_state_manager,
	public sal::tcp_client
{

// attributes
private:

	/// Statistics
	stats::stats& stats_;

	/// Protect data sending
	pthread_mutex_t* mut_;


// methods
public:

	state_manager_proxy(  conf::configuration* conf,
						  int node_to_connect,
						  stats::stats& stats,
						  pthread_mutex_t* mut);

	void
	process_state(rg::dmc_state* as) override;

private:

	state_manager_proxy(const state_manager_proxy&);
	const state_manager_proxy& operator=(const state_manager_proxy&);

};

//////////////////////////////////////////////////////////////////////

}} // namespaces

#endif
