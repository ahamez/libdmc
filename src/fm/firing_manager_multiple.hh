// $Author: lbautista $
// $Revision: 1036 $
// $Date: 2008-04-14 17:10:01 +0200 (Mon, 14 Apr 2008) $

#ifndef DMC_FM_H_
#define DMC_FM_H_

#include <pthread.h>
#include <vector>

#include "fm/firing_manager_thread.hh"
#include "rg/new_states_set.hh"
#include "fm/abstract_firing_manager.hh"
#include "sm/abstract_state_manager.hh"
#include "rg/abstract_model_factory.hh"
#include "mon/abstract_activity_monitor.hh"
#include "conf/configuration.hh"
#include "stats/stats.hh"

namespace dmc { namespace fm {

	typedef std::vector<firing_manager_thread*> fm_threads;

//////////////////////////////////////////////////////////////////////

class firing_manager_multiple
	:
	public fm::abstract_firing_manager
{

// attributes
private:

	/// The objet giving informations on the configuration of libdmc
	conf::configuration* conf_;

	/// Statistics
	stats::stats& stats_;

	/// The computing fm_threads
	fm_threads fm_threads_;

	/// The stack of new states
	rg::new_states_set* nss_;

	/// To help fm_threads to have an unique id
	tbb::atomic<unsigned int> thread_id_counter_;

	/// A mutex to protect termination condition accesses
	pthread_mutex_t* mut_;

	/// A pthread condition to wait upon until termination of fm_threads
	pthread_cond_t* termination_;


// methods
public:

	firing_manager_multiple( conf::configuration* conf,
							 stats::stats& stats,
							 rg::abstract_model_factory* model_fact,
							 rg::new_states_set* nss,
							 sm::abstract_state_manager* sm,
							 mon::abstract_activity_monitor* aam,
							 pthread_mutex_t* mut,
							 pthread_cond_t* termination);

	~firing_manager_multiple();

	void
	process_initial_state() override;

	void
	start_generation() override;

	void
	stop_generation() override;


private:

	// firing_manager_multiple cannot be copied
	firing_manager_multiple(const firing_manager_multiple& fm);
	const firing_manager_multiple& operator=(const firing_manager_multiple& fm);

};

//////////////////////////////////////////////////////////////////////

}} // namespaces

#endif
