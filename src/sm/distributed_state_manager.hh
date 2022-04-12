// $Author: lbautista $
// $Revision: 1036 $
// $Date: 2008-04-14 17:10:01 +0200 (Mon, 14 Apr 2008) $

#ifndef DMC_DIST_STATE_MANAGER_HH_
#define DMC_DIST_STATE_MANAGER_HH_

#include "rg/new_states_set.hh"
#include "mon/abstract_activity_monitor.hh"
#include "sm/state_manager_service.hh"
#include "rg/dmc_state.hh"
#include "conf/configuration.hh"

namespace dmc { namespace sm {

	typedef std::vector<sm::abstract_state_manager*> tab_sm;

//////////////////////////////////////////////////////////////////////

/// A distributed implementation of a state manager.
/// It hides the network from the user. It contains an array of state_managers,
/// one is the local state_manager (with a unicity table), the others are proxies for
/// distant state_managers. Proxies are initialized and connected on the fly as needed.
/// Local id is guessed by comparing gethostname() with the node names in the config file.

class distributed_state_manager
	:
	public sm::abstract_state_manager
{

// attributes
private:

	/// The objet giving informations on the configuration of libdmc
	conf::configuration* conf_;

	/// Statistics
	stats::stats& stats_;

	/// Protect creation of proxies
	pthread_mutex_t proxies_creation_mutex_;

	/// Protect data sending
	pthread_mutex_t send_mut_;

	/// The (pseudo) state_manager collaborating in this distributed schema.
	/// tab_sm[local_id] is a concrete state_manager with unicity table.
	/// tab_sm[i], i!=local_id is a state_manager_proxy (initially not instanciated).
	tab_sm tab_sm_;

	/// The state_manager service instanciated to handle incoming distant states.
	sm::state_manager_service* sm_server_;


// methods
public:

    distributed_state_manager( conf::configuration* conf,
							   stats::stats& stats,
                               rg::new_states_set* nss);

	~distributed_state_manager();

	/// implemented from abstract_state_manager
    void
	process_state(rg::dmc_state* as) override;

	// cannot copy distributed_state_manager
	distributed_state_manager(const distributed_state_manager&);
	distributed_state_manager& operator=(const distributed_state_manager&);

};

//////////////////////////////////////////////////////////////////////

}} // namespaces

#endif
