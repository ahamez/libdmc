// $Author: lbautista $
// $Revision: 1036 $
// $Date: 2008-04-14 17:10:01 +0200 (Mon, 14 Apr 2008) $

#include "fm/firing_manager_thread.hh"
#include "rg/abstract_model.hh"
#include "rg/dmc_state.hh"
#include "rg/abstract_state_succ_iterator.hh"

namespace dmc { namespace fm {

//////////////////////////////////////////////////////////////////////

firing_manager_thread::firing_manager_thread(	conf::configuration* conf,
												stats::stats& stats,
												rg::abstract_model_factory* model_fact,
											 	tbb::atomic<unsigned int>& thread_id_counter,
											 	rg::new_states_set* nss,
											 	sm::abstract_state_manager* sm,
											 	mon::abstract_activity_monitor* aam)
	:
	conf_(conf),
	stats_(stats),
	nss_(nss),
	sm_(sm),
	am_(nullptr),
	aam_(aam),
	should_run_(false),
	nb_processed_states_(0),
	nb_succ_(0),
	thread_id_(thread_id_counter++)
{
	// get a model instance
	am_ = model_fact->create_model();
}

//////////////////////////////////////////////////////////////////////

firing_manager_thread::~firing_manager_thread()
{
	delete am_;
}

//////////////////////////////////////////////////////////////////////

// call to ask fm to process the model initial state.
// Note that calling thread is the one that does the work in this implementation,
// so this call may take a while to return.
void
firing_manager_thread::process_initial_state()
{
	rg::dmc_state* tmp = am_->get_initial_state();
	// root state => distance = 0 !
	tmp->set_distance(0);
	sm_->process_state(tmp);
}

//////////////////////////////////////////////////////////////////////

// Non-blocking, starts a state generation loop in one independent thread.
// Creates a thread if it has not been started yet, otherwise just unblocks the running thread.
void
firing_manager_thread::start_generation()
{
	// runner thread not created yet. Create and start it.
	aam_->notify_active();
	this->start_thread();
	// thread created, let it run if it was stopped before
	should_run_.set_state(true);
}

//////////////////////////////////////////////////////////////////////

void
firing_manager_thread::stop_generation()
{
	nss_->signal_empty();
}

//////////////////////////////////////////////////////////////////////

/// The body of a firing thread execution
void
firing_manager_thread::run()
{
	// BFS walk
	while(true)
	{
		rg::dmc_state* tmp_state;

		nss_->lock();

		bool is_empty = nss_->is_empty();

		if( is_empty )
		{
			aam_->notify_inactive();
		}

		while( nss_->is_empty() )
		{
			nss_->wait_non_empty();
		}

		tmp_state = nss_->pop_state();

		if( is_empty )
		// if ever we said that we were inactive in the previous test on
		// is_empty
		{
			aam_->notify_active();
		}

		nss_->unlock();

		stats_.notify_state_processed();

		// compute successors of the state using the model given by the user
		rg::abstract_state_succ_iterator* it = am_->get_succ_iterator(tmp_state);

		while( it->has_successors() )
		{
			// not a c++ like iterator :-)
			rg::dmc_state* tmp_succ = it->get_next_successor();
			++nb_succ_;
			stats_.notify_successor_processed();

			tmp_succ->set_distance( tmp_state->get_distance() + 1 );

			// process the potentially new states
			sm_->process_state( tmp_succ );

		}

		delete it;

	}
}

//////////////////////////////////////////////////////////////////////

}} // namespaces
