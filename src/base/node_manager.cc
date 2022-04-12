// $Author: lbautista $
// $Revision: 1036 $
// $Date: 2008-04-14 17:10:01 +0200 (Mon, 14 Apr 2008) $

#include <vector>

#include "node_manager.hh"
#include "fm/firing_manager_multiple.hh"
#include "sm/distributed_state_manager.hh"
#include "sm/state_manager.hh"
#include "util/hash.hh"

namespace dmc { namespace base {

//////////////////////////////////////////////////////////////////////

node_manager::node_manager( conf::configuration* conf,
							stats::stats& stats,
							rg::abstract_model_factory* amf,
							mon::abstract_activity_monitor* upper_bam,
							rg::new_states_set* nss)
	:
	conf_(conf),
	stats_(stats),
	nss_(nss),
	sm_(),
	amf_(amf),
	fm_(),
	fm_bam_(0),
	upper_bam_(upper_bam),
	generate_(true),
	termination_mutex_(),
	termination_cond_()
{
	// init hash functions of unicity table
	util::hash::init();

	// what kind of state_manager do we need?
	if( conf_->generation_is_distributed() )
	{
		sm_ = new sm::distributed_state_manager( conf_,
												 stats_,
												 nss_);
	}
	else
	{
		sm_ = new sm::state_manager(	conf_,
										nss_);
	}

	fm_ = new fm::firing_manager_multiple( conf_,
										   stats_,
										   amf_,
										   nss_,
										   sm_,
										   &fm_bam_,
										   &termination_mutex_,
										   &termination_cond_);

	pthread_mutex_init(&termination_mutex_,nullptr);
	pthread_cond_init(&termination_cond_, nullptr);
}

//////////////////////////////////////////////////////////////////////

node_manager::~node_manager()
{
	delete fm_;
	delete sm_;
	delete amf_;

	pthread_mutex_destroy(&termination_mutex_);
	pthread_cond_destroy(&termination_cond_);
}

//////////////////////////////////////////////////////////////////////

void
node_manager::process_initial_state()
{
	fm_->process_initial_state();
}

//////////////////////////////////////////////////////////////////////

void
node_manager::start_generation()
{
	upper_bam_->notify_active();
	// create a new thread
	this->start_thread();
}

//////////////////////////////////////////////////////////////////////

void
node_manager::stop_generation()
{
	generate_ = false;

	pthread_mutex_lock(&termination_mutex_);

	if( !fm_bam_.is_active() )
	{
		fm_bam_.notify_active();
	}
	else
	{
		fm_bam_.set_inactive();
	}

	// we are waiting for the end of the fm_threads
	// otherwise objects in which they are running would
	// be deleted
	pthread_cond_wait( &termination_cond_,
					   &termination_mutex_);

	pthread_mutex_unlock(&termination_mutex_);
}

//////////////////////////////////////////////////////////////////////

void
node_manager::run()
{
	fm_->start_generation();

	if( conf_->generation_is_distributed() )
	{
		while(generate_)
		{
			fm_bam_.wait_until_inactive();
			if( !generate_ )
			{
				break;
			}
			upper_bam_->notify_inactive();

			fm_bam_.wait_until_active();
			if( !generate_ )
			{
				break;
			}
			upper_bam_->notify_active();
		}
	}
	else
	{
		while(generate_)
		{
			fm_bam_.wait_until_inactive();

			if( !generate_ )
			{
				break;
			}

			nss_->lock();

			if( nss_->is_empty() )
			{
				if( !fm_bam_.is_active() )
				{
					upper_bam_->notify_inactive();
				}
			}

			nss_->unlock();

			fm_bam_.wait_until_active();

			if( !generate_ )
			{
				break;
			}

			upper_bam_->notify_active();
		}

	}

	fm_->stop_generation();
	stop_thread();
}

//////////////////////////////////////////////////////////////////////

base::status_t
node_manager::get_status()
{
	base::status_t res;

	nss_->lock();
	res.empty_set = nss_->is_empty();
	nss_->unlock();

	res.nb_sent = stats_.get_nb_sent_states();
	res.nb_recv = stats_.get_nb_received_states();

	return res;
}

//////////////////////////////////////////////////////////////////////

base::stats_t*
node_manager::get_stats()
{
	auto   res = new base::stats_t;

	res->set_size = nss_->get_size();
	res->nb_states = conf_->ut_->get_size();
	res->nb_succ = stats_.get_nb_processed_successors();
	res->sent_nodes = new unsigned long[conf_->get_nb_nodes()];

	for( int i = 0 ; i < conf_->get_nb_nodes() ; ++i )
	{
		res->sent_nodes[i] = stats_.get_nb_states_sent_to_node(i);
	}

	return res;
}

//////////////////////////////////////////////////////////////////////

}} // namespaces
