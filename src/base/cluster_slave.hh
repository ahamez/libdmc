// $Author: lbautista $
// $Revision: 1036 $
// $Date: 2008-04-14 17:10:01 +0200 (Mon, 14 Apr 2008) $

#ifndef DMC_CLUSTER_SLAVE_HH_
#define DMC_CLUSTER_SLAVE_HH_

#include <pthread.h>
#include <vector>

#include "rg/new_states_set.hh"
#include "mon/activity_monitor_proxy.hh"
#include "mon/activity_monitor_service.hh"
#include "fm/firing_manager_service.hh"
#include "rg/abstract_model_factory.hh"
#include "stats/stats.hh"

namespace dmc { namespace base {

//////////////////////////////////////////////////////////////////////

class cluster_slave
	:
	public fm::abstract_firing_manager
{

// attributes
private:

	/// The objet giving informations on the configuration of libdmc
	conf::configuration* conf_;

	/// Statistics
	stats::stats& stats_;

	/// The local node_manager
	base::abstract_node* local_node_;

	/// The service listening for order coming from the master node
	fm::firing_manager_service* fm_server_;

	/// The monitor on the slave node observing distributed activity
	mon::activity_monitor_proxy* bam_proxy_;

	/// The set of new states to process
	rg::new_states_set nss_;

// methods
public:

	cluster_slave( conf::configuration* conf,
				   stats::stats& stats,
				   rg::abstract_model_factory* model_fact);

	virtual ~cluster_slave();

	void
	process_initial_state() override;

	void
	start_generation() override;

	void
	stop_generation() override;


private:

	// cannot copy dmc_manager
	cluster_slave(const cluster_slave&);
	const cluster_slave& operator=(const cluster_slave&);

};

	//////////////////////////////////////////////////////////////////////

}} // namespaces

#endif
