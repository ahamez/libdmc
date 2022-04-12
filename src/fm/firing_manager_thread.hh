// $Author: lbautista $
// $Revision: 1036 $
// $Date: 2008-04-14 17:10:01 +0200 (Mon, 14 Apr 2008) $

#ifndef DMC_FM_THREAD_H_
#define DMC_FM_THREAD_H_

#include "rg/abstract_model.hh"
#include "rg/abstract_model_factory.hh"
#include "fm/abstract_firing_manager.hh"
#include "rg/new_states_set.hh"
#include "sm/abstract_state_manager.hh"
#include "mon/abstract_activity_monitor.hh"
#include "mon/barrier_condition.hh"
#include "sal/thread.hh"
#include "conf/configuration.hh"
#include "stats/stats.hh"

namespace dmc { namespace fm {

//////////////////////////////////////////////////////////////////////

/// A class to run a single thread to compute new states.
/// Implements the fm interface and interface runnable for class thread.

class firing_manager_thread
	:
	public sal::thread,
	public fm::abstract_firing_manager
{

// attributes
private :

	/// The objet giving informations on the configuration of libdmc
	conf::configuration* conf_;

	/// Statistics
	stats::stats& stats_;

	/// the nss used for pop
	rg::new_states_set* nss_;

	/// the sm used for process_state
	sm::abstract_state_manager* sm_;

	/// the model used by this thread
	rg::abstract_model* am_;

	/// the monitor interested in activity of this thread
	mon::abstract_activity_monitor* aam_;

	/// to handle interruption and resume of thread
	mon::barrier_condition should_run_;

	/// Number of unique states processed by a single thread
	int nb_processed_states_;

	/// Number of states processed by a single thread
	int nb_succ_;

	/// Hold next id to affect to a thread
	unsigned int thread_id_;


// methods
public:

	/// Create a model using the factory, link to fss and sm.
	firing_manager_thread( conf::configuration* conf,
						   stats::stats& stats,
                           rg::abstract_model_factory* model_fact,
						   tbb::atomic<unsigned int>& thread_id_counter,
						   rg::new_states_set* nss,
						   sm::abstract_state_manager* sm,
						   mon::abstract_activity_monitor* aam);

	virtual
	~firing_manager_thread();

	void
	process_initial_state() override;

	void
	start_generation() override;

	void
	stop_generation() override;

	// Required from interface thread
	void run() override;

private:

	// firing_manager_thread cannot be copied
	firing_manager_thread(const firing_manager_thread& fm);
	const firing_manager_thread& operator=(const firing_manager_thread& fm);

};

//////////////////////////////////////////////////////////////////////

}} // namespaces

#endif
