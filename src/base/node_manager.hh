// $Author: lbautista $
// $Revision: 1036 $
// $Date: 2008-04-14 17:10:01 +0200 (Mon, 14 Apr 2008) $

#ifndef DMC_NODE_MANAGER_H_
#define DMC_NODE_MANAGER_H_

#include <pthread.h>

#include "base/abstract_node.hh"
#include "fm/abstract_firing_manager.hh"
#include "fm/firing_manager_multiple.hh"
#include "rg/new_states_set.hh"
#include "mon/abstract_activity_monitor.hh"
#include "mon/barrier_integer_activity_monitor.hh"
#include "conf/configuration.hh"
#include "sal/thread.hh"
#include "stats/stats.hh"

namespace dmc { namespace base {

//////////////////////////////////////////////////////////////////////

class node_manager
	:
	public sal::thread,
	public base::abstract_node
{

// attributes
private:

	/// The objet giving informations on the configuration of libdmc
	conf::configuration* conf_;

	/// Statistics
	stats::stats& stats_;

	/// A state stack of states to be treated
	rg::new_states_set* nss_;

	/// A state_manager for unicity
	sm::abstract_state_manager* sm_;

	/// A model factory to create firing managers
	rg::abstract_model_factory* amf_;

	/// A firing manager
	fm::firing_manager_multiple* fm_;

	/// An activity monitor for the mutliple fms
	mon::barrier_integer_activity_monitor fm_bam_;

	/// The monitor to which this class must report
	mon::abstract_activity_monitor* upper_bam_;

	/// Indicate if the node has to generate
	bool generate_;

	/// A mutex to protect termination condition accesses
	pthread_mutex_t termination_mutex_;

	/// A pthread condition to wait upon until termination of fm_threads
	pthread_cond_t termination_cond_;


// methods
public:

	node_manager( conf::configuration* conf,
				  stats::stats& stats,
				  rg::abstract_model_factory* amf,
				  mon::abstract_activity_monitor* upper_bam,
				  rg::new_states_set* nss);

	~node_manager();

	// required from interface firing_manager
	void
	process_initial_state() override;

	void
	start_generation() override;

	void
	stop_generation() override;

	// required from interface thread
	void run() override;

	base::status_t
	get_status() override;

	base::stats_t*
	get_stats() override;


private:

	// cannot copy node_manager
	node_manager(const node_manager&);
	const node_manager& operator=(const node_manager&);

};

//////////////////////////////////////////////////////////////////////

}} // namespaces

#endif
