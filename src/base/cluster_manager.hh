// $Author: lbautista $
// $Revision: 1036 $
// $Date: 2008-04-14 17:10:01 +0200 (Mon, 14 Apr 2008) $

#ifndef DMC_CLUSTER_MANAGER_HH_
#define DMC_CLUSTER_MANAGER_HH_

#include <vector>

#include "base/abstract_node.hh"
#include "fm/abstract_firing_manager.hh"
#include "mon/barrier_integer_activity_monitor.hh"
#include "mon/activity_monitor_service.hh"
#include "fm/firing_manager_service.hh"
#include "rg/abstract_model_factory.hh"
#include "conf/configuration.hh"
#include "stats/stats.hh"

namespace dmc { namespace base {

//////////////////////////////////////////////////////////////////////

class cluster_manager
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
	base::abstract_node* fm_;

	/// Contains all nodes
	nodes nodes_;

	/// An activity monitor
	mon::barrier_integer_activity_monitor* bam_;

	/// The monitor on the master node observing distributed activity
	mon::activity_monitor_service* bam_server_;

	/// model factory
	rg::abstract_model_factory* model_fact_;

	/// The set of new states to process
	rg::new_states_set nss_;

// methods
public:

	cluster_manager( conf::configuration* conf,
					 stats::stats& stats,
					 rg::abstract_model_factory* model_fact);

	~cluster_manager();

	void
	process_initial_state() override;

	void
	start_generation() override;

	void
	stop_generation() override;

	//////////////////////////////////////////////////////////////////////

private:

	/// The treatment is the same, whatever is the *_generation() invoked
	template <typename T>
	void
	process(const T& x);

	/// Verify if generation is really finished
	bool
	generation_has_ended();

	// cannot copy dmc_manager
	cluster_manager(const cluster_manager&);
	const cluster_manager& operator=(const cluster_manager&);

};

//////////////////////////////////////////////////////////////////////

}} // namespaces

#endif
