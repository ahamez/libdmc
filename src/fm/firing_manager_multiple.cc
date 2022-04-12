// $Author: lbautista $
// $Revision: 1036 $
// $Date: 2008-04-14 17:10:01 +0200 (Mon, 14 Apr 2008) $

#include "fm/firing_manager_multiple.hh"
#include "rg/abstract_model.hh"
#include "rg/dmc_state.hh"
#include "rg/abstract_state_succ_iterator.hh"

// if only I had a computer with so many processors :-)
#ifndef DMC_MAX_FM_THREADS
#define DMC_MAX_FM_THREADS 32
#endif

namespace dmc { namespace fm {

//////////////////////////////////////////////////////////////////////

firing_manager_multiple::firing_manager_multiple( conf::configuration* conf,
												  stats::stats& stats,
												  rg::abstract_model_factory* model_fact,
												  rg::new_states_set* nss,
												  sm::abstract_state_manager* sm,
												  mon::abstract_activity_monitor* aam,
												  pthread_mutex_t* mut,
												  pthread_cond_t* termination)
	:
	conf_(conf),
	stats_(stats),
	fm_threads_(),
	nss_(nss),
	thread_id_counter_(),
	mut_(mut),
	termination_(termination)
{
	int nb_fm_threads = conf_->get_nb_threads();
	thread_id_counter_ = 0;

	if( nb_fm_threads > 1)
	{
		// normalization
		if( nb_fm_threads > DMC_MAX_FM_THREADS )
		{
			nb_fm_threads = DMC_MAX_FM_THREADS;
		}
	}

	fm_threads_.reserve(nb_fm_threads);
	for( int i = 0 ; i < nb_fm_threads ; ++i )
	{
		fm_threads_.push_back( new firing_manager_thread( conf,
					    			  stats_,
                                                                  model_fact,
								  thread_id_counter_,
								  nss,
								  sm,
								  aam));
	}
}

//////////////////////////////////////////////////////////////////////

firing_manager_multiple::~firing_manager_multiple()
{
	for(auto & elem : fm_threads_)
	{
		delete elem;
	}

	fm_threads_.clear();
}

//////////////////////////////////////////////////////////////////////

/// Tells a fm to start_generation the first state of the state space

void
firing_manager_multiple::process_initial_state()
{
	if( conf_->get_nb_threads() > 0 )
	{
		fm_threads_[0]->process_initial_state();
	}
	else
	{
		exit(EXIT_FAILURE);
	}
}

//////////////////////////////////////////////////////////////////////

void
firing_manager_multiple::start_generation()
{
        for(auto & elem : fm_threads_)
	{
		(elem)->start_generation();
	}
}

//////////////////////////////////////////////////////////////////////

void
firing_manager_multiple::stop_generation()
{
 	for(auto & elem : fm_threads_)
	{
            (elem)->kill_thread();
	}

	pthread_mutex_lock(mut_);
	pthread_cond_broadcast(termination_);
	pthread_mutex_unlock(mut_);
}

//////////////////////////////////////////////////////////////////////


}} // namespaces
